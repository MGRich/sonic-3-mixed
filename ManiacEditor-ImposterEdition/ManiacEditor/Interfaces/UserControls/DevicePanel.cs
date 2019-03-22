﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Diagnostics;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using SharpDX;
using SharpDX.Direct3D9;
using SharpDX.Windows;
using Font = SharpDX.Direct3D9.Font;
using Rectangle = System.Drawing.Rectangle;
using Color = System.Drawing.Color;
using Point = System.Drawing.Point;
using Bitmap = System.Drawing.Bitmap;


/*
 * This file is a ported old rendering code
 * It will be replaced with OpenGL soon (Update: Or never if this pace keeps up - CarJem Generations)
 */


namespace ManiacEditor
{
    public partial class DevicePanel : UserControl
    {
        public Editor EditorInstance;

        #region Members

        public bool mouseMoved = false;
        public bool renderInProgress = false;
        DialogResult deviceExceptionResult;

        public int DrawWidth;
        public int DrawHeight;

        public int ScreenPosWidth;
        public int ScreenPosHeight;

        private Sprite sprite;
		private Sprite sprite2;
		private Sprite HUD;
        Texture tx;
        Bitmap txb;
		Texture hvcursor;
		Bitmap hvcursorb;
		Texture vcursor;
		Bitmap vcursorb;
		Texture hcursor;
		Bitmap hcursorb;


		public bool bRender = true;
		public bool tRender = false;

		// The DirectX device
		public Device _device = null;
        public bool deviceLost;
        private Direct3D direct3d = new Direct3D();
        private Font font;
        private Font fontBold;
        public VertexBuffer vb;
        // The Form to place the DevicePanel onto
        public IDrawArea _parent = null;
        private PresentParameters presentParams;
        // On this event we can start to render our scene
        public event RenderEventHandler OnRender;

        // Now we know that the device is created
        public event CreateDeviceEventHandler OnCreateDevice;

        public MouseEventArgs lastEvent;

        

        #endregion

        #region Constructor

        public DevicePanel(Editor instance = null)
        {
            InitializeComponent();
            EditorInstance = instance;
        }

        #endregion

        #region Init DX

        /// <summary>
        /// Init the DirectX-Stuff here
        /// </summary>
        /// <param name="parent">parent of the DevicePanel</param>
        /// 

        public void Init(IDrawArea parent)
        {
            try
            {
                _parent = parent;

                // Setup our D3D stuff
                presentParams = new PresentParameters();
                presentParams.Windowed = true;
                presentParams.SwapEffect = SwapEffect.Discard;

                Capabilities caps = direct3d.Adapters.First().GetCaps(DeviceType.Hardware);

                CreateFlags createFlags;

                if ((caps.DeviceCaps & DeviceCaps.HWTransformAndLight) != 0)
                {
                    createFlags = CreateFlags.HardwareVertexProcessing;
                }
                else
                {
                    createFlags = CreateFlags.SoftwareVertexProcessing;
                }

                if ((caps.DeviceCaps & DeviceCaps.PureDevice) != 0 && createFlags ==
                    CreateFlags.HardwareVertexProcessing)
                {
                    createFlags |= CreateFlags.PureDevice;
                }


                _device = new Device(direct3d, 0, DeviceType.Hardware, this.Handle, createFlags, presentParams);
                _device.SetSamplerState(0, SamplerState.MinFilter, TextureFilter.None);
                _device.SetSamplerState(0, SamplerState.MagFilter, TextureFilter.None);
                _device.SetSamplerState(0, SamplerState.MipFilter, TextureFilter.None);
                _device.SetRenderState(RenderState.ZWriteEnable, false);
                _device.SetRenderState(RenderState.ZEnable, false);

				if (OnCreateDevice != null)
                {
                    OnCreateDevice(this, new DeviceEventArgs(_device));
                }



				txb = new Bitmap(1, 1);
                using (Graphics g = Graphics.FromImage(txb))
                    g.Clear(Color.White);
				hcursorb = new Bitmap(32, 32);
				using (Graphics g = Graphics.FromImage(hcursorb))
					Cursors.NoMoveHoriz.Draw(g, new Rectangle(0, 0, 32, 32));
				MakeGray(hcursorb);

				vcursorb = new Bitmap(32, 32);
				using (Graphics g = Graphics.FromImage(vcursorb))
					Cursors.NoMoveVert.Draw(g, new Rectangle(0, 0, 32, 32));
				MakeGray(vcursorb);

				hvcursorb = new Bitmap(32, 32);
				using (Graphics g = Graphics.FromImage(hvcursorb))
					Cursors.NoMove2D.Draw(g, new Rectangle(0, 0, 32, 32));
				MakeGray(hvcursorb);
				if (parent != null) InitDeviceResources();

            }
            catch (SharpDXException ex)
            {
                try
                {
                    ResetDevice();
                }
                catch
                {
                    throw new ArgumentException("Error initializing DirectX", ex);
                }

            }
            catch (DllNotFoundException)
            {
                throw new Exception("Please install DirectX Redistributable June 2010");
            }
        }



        public void Run()
        {
			
			RenderLoop.Run(this, () =>
			{
				// Another option is not use RenderLoop at all and call Render when needed, and call here every tick for animations
				if (bRender || tRender)
				{
					Render();
					//bRender = false;
				}
				if (mouseMoved)
				{
					OnMouseMove(lastEvent);
					mouseMoved = false;
				}
				// Application.DoEvents();
			});
		}

        public void InitDeviceResources()
        {
            sprite = new Sprite(_device);
            sprite2 = new Sprite(_device);
            HUD = new Sprite(_device);

            tx = TextureCreator.FromBitmap(_device, txb);
            //tcircle = TextureCreator.FromBitmap(_device, tcircleb);
            //tecircle = TextureCreator.FromBitmap(_device, tecircleb);
            hcursor = TextureCreator.FromBitmap(_device, hcursorb);
            vcursor = TextureCreator.FromBitmap(_device, vcursorb);
            hvcursor = TextureCreator.FromBitmap(_device, hvcursorb);

            FontDescription fontDescription = new FontDescription()
            {
                Height = 18,
                Italic = false,
                CharacterSet = FontCharacterSet.Ansi,
                FaceName = "Microsoft Sans Serif",
                MipLevels = 0,
                OutputPrecision = FontPrecision.TrueType,
                PitchAndFamily = FontPitchAndFamily.Default,
                Quality = FontQuality.Antialiased,
                Weight = FontWeight.Regular
            };

            FontDescription fontDescriptionBold = new FontDescription()
            {
                Height = 18,
                Italic = false,
                CharacterSet = FontCharacterSet.Ansi,
                FaceName = "Microsoft Sans Serif",
                MipLevels = 0,
                OutputPrecision = FontPrecision.TrueType,
                PitchAndFamily = FontPitchAndFamily.Default,
                Quality = FontQuality.Antialiased,
                Weight = FontWeight.Bold
            };

            font = new Font(_device, fontDescription);
            fontBold = new Font(_device, fontDescriptionBold);
        }

        public void ResetDevice()
        {
            DisposeDeviceResources();
            _parent.DisposeTextures();
            try { _device.Reset(presentParams); }
            catch { Init(_parent); }
            InitDeviceResources();
            if (EditorInstance.EditorScene != null) EditorInstance.ReloadToolStripButton_Click(null, null);
            deviceLost = false;
         }

        private void MakeGray(Bitmap image)
        {
            for (int x = 0; x < image.Width; x++)
            {
                for (int y = 0; y < image.Height; y++)
                {
                    if (image.GetPixel(x, y).Name == "ffffffff")
                        image.SetPixel(x, y, Color.Transparent);
                    else if (image.GetPixel(x, y).Name == "ff000000")
                        image.SetPixel(x, y, Color.Gray);
                }
            }
        }

        #endregion

        #region Properties

        /// <summary>
        /// Extend this list of properties if you like
        /// </summary>
        private Color _deviceBackColor = Color.Black;

        public Color DeviceBackColor
        {
            get { return _deviceBackColor; }
            set { _deviceBackColor = value; }
        }


        #endregion

        #region Rendering

        public void DeviceExceptionDialog(int state, SharpDXException ex, SharpDXException ex2, AccessViolationException ex3 = null)
        {
            String error = "No Error. OH-NO!";
            String error2 = "";
            String error3 = "";
            String errorCode = "404 (No Error Code Found)";
            String errorCode2 = "404 (No Secondary Error Code Found)";
            String errorCode3 = "404 (No Trimary Error Code Found)";

            if (ex != null)
            {
                 error = ex.ToString();
                 errorCode = ex.ResultCode.ToString();
            }
            if (ex3 != null)
            {
                error3 = ex3.ToString();
                errorCode3 = ex3.HResult.ToString();
            }
            if (ex2 != null)
            {
                 error2 = ex2.ToString();
                 errorCode2 = ex2.ResultCode.ToString();
            }

            if (state == 0)
            {
                    using (var deviceLostBox = new DeviceLostBox(error, error2, error3, errorCode, errorCode2, error3))
                    {
                        deviceLostBox.ShowDialog();
                        deviceExceptionResult = deviceLostBox.DialogResult;
                    }
                    if (deviceExceptionResult == DialogResult.Yes) //Yes and Exit
                    {
                        EditorInstance.BackupSceneBeforeCrash();
                        Environment.Exit(1);

                    }
                    else if (deviceExceptionResult == DialogResult.No) //No and try to Restart
                    {
                        DisposeDeviceResources();
                        Init(EditorInstance.GraphicsModel);

                    }
                    else if (deviceExceptionResult == DialogResult.Retry) //Yes and try to Restart
                    {
                        EditorInstance.BackupSceneBeforeCrash();
                        DisposeDeviceResources();
                        Init(EditorInstance.GraphicsModel);
                    }
                    else if (deviceExceptionResult == DialogResult.Ignore) //No and Exit
                    {
                        Environment.Exit(1);
                    }
            }
            else if (state == 1)
            {

                using (var deviceLostBox = new DeviceLostBox(error, error2, error3, errorCode, errorCode2, errorCode3, 1))
                {
                    deviceLostBox.ShowDialog();
                    deviceExceptionResult = deviceLostBox.DialogResult;

                }
                if (deviceExceptionResult == DialogResult.Yes) //Yes and Exit
                {
                    EditorInstance.BackupSceneBeforeCrash();
                    Environment.Exit(1);

                }
                else if (deviceExceptionResult == DialogResult.No) //No and try to Restart
                {
                    DisposeDeviceResources();
                    Init(EditorInstance.GraphicsModel);

                }
                else if (deviceExceptionResult == DialogResult.Retry) //Yes and try to Restart
                {
                    EditorInstance.BackupSceneBeforeCrash();
                    DisposeDeviceResources();
                    Init(EditorInstance.GraphicsModel);
                }
                else if (deviceExceptionResult == DialogResult.Ignore) //No and Exit
                {
                    Environment.Exit(1);
                }
            }
        }

        /// <summary>
        /// Rendering-method
        /// </summary>
        public void Render()
        {


			if (Parent == null || _device == null || deviceLost || direct3d == null) return;


            try
            {
                Rectangle screen = _parent.GetScreen();
                double zoom = _parent.GetZoom();

                //Clear the backbuffer
                _device.Clear(ClearFlags.Target, new SharpDX.Color(_deviceBackColor.R, _deviceBackColor.B, _deviceBackColor.G, _deviceBackColor.A), 1.0f, 0);

                //Begin the scene
                _device.BeginScene();

                sprite.Transform = Matrix.Scaling((float)zoom, (float)zoom, 1f);

                if (EditorInstance.UseLargeDebugStats) HUD.Transform = Matrix.Scaling(2f, 2f, 2f);
                else HUD.Transform = Matrix.Scaling(1f, 1f, 1f);
				sprite2.Begin(SpriteFlags.AlphaBlend);


				var state1 = _device.GetSamplerState(0, SamplerState.MinFilter);
				var state2 = _device.GetSamplerState(0, SamplerState.MagFilter);
				var state3 = _device.GetSamplerState(0, SamplerState.MipFilter);
				// If zoomin, just do near-neighbor scaling
				_device.SetSamplerState(0, SamplerState.MinFilter, TextureFilter.None);
                _device.SetSamplerState(0, SamplerState.MagFilter, TextureFilter.None);
                _device.SetSamplerState(0, SamplerState.MipFilter, TextureFilter.None);

                HUD.Begin(SpriteFlags.AlphaBlend);

				_device.SetSamplerState(0, SamplerState.MinFilter, state1);
				_device.SetSamplerState(0, SamplerState.MagFilter, state2);
				_device.SetSamplerState(0, SamplerState.MipFilter, state3);
				if (zoom > 1)
                {
                    // If zoomin, just do near-neighbor scaling
                    _device.SetSamplerState(0, SamplerState.MinFilter, TextureFilter.None);
                    _device.SetSamplerState(0, SamplerState.MagFilter, TextureFilter.None);
                    _device.SetSamplerState(0, SamplerState.MipFilter, TextureFilter.None);
                }
                sprite.Begin(SpriteFlags.AlphaBlend | SpriteFlags.SortDepthFrontToBack | SpriteFlags.DoNotModifyRenderState);

                // Render of scene here
                if (OnRender != null) OnRender(this, new DeviceEventArgs(_device));

                sprite.Transform = Matrix.Scaling(1f, 1f, 1f);
                HUD.Transform = Matrix.Scaling(1f, 1f, 1f);


				Rectangle rect1 = new Rectangle(DrawWidth - screen.X, 0, Width - DrawWidth, Height);
				rect1.Intersect(new Rectangle(0, 0, screen.Width, screen.Height));
				Rectangle rect2 = new Rectangle(0, DrawHeight - screen.Y, DrawWidth, Height - DrawHeight);
				rect2.Intersect(new Rectangle(0, 0, screen.Width, screen.Height));
				DrawTexture(tx, new Rectangle(0, 0, rect1.Width, rect1.Height), new Vector3(0, 0, 0), new Vector3(rect1.X, rect1.Y, 0), SystemColors.Control);
				DrawTexture(tx, new Rectangle(0, 0, rect2.Width, rect2.Height), new Vector3(0, 0, 0), new Vector3(rect2.X, rect2.Y, 0), SystemColors.Control);
				sprite.End();
				sprite2.End();
				HUD.End();
                //End the scene
                _device.EndScene();
                _device.Present();

            }
			catch (SharpDXException ex)
			{
                ResetDevice();
			}
}

		#endregion

		#region Overrides

		protected override void OnPaint(PaintEventArgs e)
		{
			this.Render();
		}

		protected override bool IsInputKey(Keys keyData)
		{
			switch (keyData)
			{
				case Keys.Up: return true;
				case Keys.Down: return true;
				case Keys.Right: return true;
				case Keys.Left: return true;
				case Keys.PageUp: return true;
				case Keys.PageDown: return true;
			}
			return base.IsInputKey(keyData);
		}
		protected override Point ScrollToControl(Control activeControl)
        {
            return this.AutoScrollPosition;
        }

		protected override void OnMouseMove(System.Windows.Forms.MouseEventArgs e)
		{
			lastEvent = e;
			base.OnMouseMove(new MouseEventArgs(e.Button, e.Clicks, e.X + _parent.GetScreen().X, e.Y + _parent.GetScreen().Y, e.Delta));
			bRender = true;
		}

		protected override void OnMouseWheel(System.Windows.Forms.MouseEventArgs e)
		{
			base.OnMouseWheel(new MouseEventArgs(e.Button, e.Clicks, e.X + _parent.GetScreen().X, e.Y + _parent.GetScreen().Y, e.Delta));
			bRender = true;
		}

		protected override void OnMouseClick(System.Windows.Forms.MouseEventArgs e)
		{
			base.OnMouseClick(new MouseEventArgs(e.Button, e.Clicks, e.X + _parent.GetScreen().X, e.Y + _parent.GetScreen().Y, e.Delta));
			bRender = true;
		}

		protected override void OnMouseDoubleClick(System.Windows.Forms.MouseEventArgs e)
		{
			base.OnMouseDoubleClick(new MouseEventArgs(e.Button, e.Clicks, e.X + _parent.GetScreen().X, e.Y + _parent.GetScreen().Y, e.Delta));
			bRender = true;
		}

		protected override void OnMouseDown(System.Windows.Forms.MouseEventArgs e)
		{
			base.OnMouseDown(new MouseEventArgs(e.Button, e.Clicks, e.X + _parent.GetScreen().X, e.Y + _parent.GetScreen().Y, e.Delta));
			bRender = true;
		}

		protected override void OnMouseUp(System.Windows.Forms.MouseEventArgs e)
		{
			base.OnMouseUp(new MouseEventArgs(e.Button, e.Clicks, e.X + _parent.GetScreen().X, e.Y + _parent.GetScreen().Y, e.Delta));
			bRender = true;
		}

		#endregion

		public Rectangle GetScreen()
        {
            Rectangle screen = _parent.GetScreen();
            double zoom = _parent.GetZoom();
            if (zoom == 1.0)
                return screen;
            else
                return new Rectangle((int)Math.Floor(screen.X / zoom),
                    (int)Math.Floor(screen.Y / zoom),
                    (int)Math.Ceiling(screen.Width / zoom),
                    (int)Math.Ceiling(screen.Height / zoom));
        }

        public bool IsObjectOnScreen(int x, int y, int width, int height)
        {
            Rectangle screen = _parent.GetScreen();
            double zoom = _parent.GetZoom();
            if (zoom == 1.0)
                return !(x > screen.X + screen.Width
                || x + width < screen.X
                || y > screen.Y + screen.Height
                || y + height < screen.Y);
            else
                return !(x * zoom > screen.X + screen.Width
                || (x + width) * zoom < screen.X
                || y * zoom > screen.Y + screen.Height
                || (y + height) * zoom < screen.Y);
        }

        private void DrawTexture(Texture image, Rectangle srcRect, Vector3 center, Vector3 position, Color color)
        {
            sprite.Draw(image, new SharpDX.Color(color.R, color.G, color.B, color.A), new SharpDX.Rectangle(srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height), center, position);        
        }
		private void DrawTexture2(Texture image, Rectangle srcRect, Vector3 center, Vector3 position, Color color)
		{
			sprite2.Draw(image, new SharpDX.Color(color.R, color.G, color.B, color.A), new SharpDX.Rectangle(srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height), center, position);

		}

		private void DrawHUD(Texture image, Rectangle srcRect, Vector3 center, Vector3 position, Color color)
        {
            HUD.Draw(image, new SharpDX.Color(color.R, color.G, color.B, color.A), new SharpDX.Rectangle(srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height), center, position);
        }

        public void DrawBitmap(Texture image, int x, int y, int width, int height, bool selected, int transparency)
        {
            Rectangle screen = _parent.GetScreen();
            double zoom = _parent.GetZoom();
            DrawTexture(image, new Rectangle(0, 0, width, height), new Vector3(), new Vector3(x - (int)(screen.X / zoom), y - (int)(screen.Y / zoom), 0), (selected) ? Color.BlueViolet : Color.FromArgb(transparency, Color.White));
        }

		public void DrawHUDRectangle(int x1, int y1, int x2, int y2, Color color)
		{
			Rectangle screen = _parent.GetScreen();
			double zoom = _parent.GetZoom();
			DrawHUD(tx, new Rectangle(0, 0, x2 - x1, y2 - y1), new Vector3(), new Vector3(x1, y1, 0), color);
		}

		public void DrawHUDBitmap(Texture image, int x, int y, int width, int height, bool selected, int transparency)
        {
            Rectangle screen = _parent.GetScreen();
            double zoom = _parent.GetZoom();
            DrawHUD(image, new Rectangle(0, 0, width, height), new Vector3(), new Vector3(x, y, 0), (selected) ? Color.BlueViolet : Color.FromArgb(transparency, Color.White));
        }

        public void DrawLine(int X1, int Y1, int X2, int Y2, Color color = new Color())
        {
            Rectangle screen = _parent.GetScreen();
            double zoom = _parent.GetZoom();
            int width = Math.Abs(X2 - X1);
            int height = Math.Abs(Y2 - Y1);
            int x = Math.Min(X1, X2);
            int y = Math.Min(Y1, Y2);
            int pixel_width = Math.Max((int)zoom, 1);

            if (!IsObjectOnScreen(x, y, width, height)) return;


            sprite.Transform = Matrix.Scaling(1f, 1f, 1f); 
            if (width == 0 || height == 0)
            {
                if (width == 0) width = pixel_width;
                else width = (int)(width * zoom);
                if (height == 0) height = pixel_width;
                else height = (int)(height * zoom);
                DrawTexture(tx, new Rectangle(0, 0, width, height), new Vector3(0, 0, 0), new Vector3((int)((x - (int)(screen.X / zoom)) * zoom), (int)((y - (int)(screen.Y / zoom)) * zoom), 0), color);
            }
            else
            {
                DrawLinePBP(X1, Y1, X2, Y2, color);
            }
            sprite.Transform = Matrix.Scaling((float)zoom, (float)zoom, 1f);
        }

        public void DrawLinePaperRoller(int X1, int Y1, int X2, int Y2, Color color, Color color2, Color color3, Color color4)
        {
            Rectangle screen = _parent.GetScreen();
            double zoom = _parent.GetZoom();
            int width = Math.Abs(X2 - X1);
            int height = Math.Abs(Y2 - Y1);
            int x = Math.Min(X1, X2);
            int y = Math.Min(Y1, Y2);
            int pixel_width = Math.Max((int)zoom, 1);

            if (!IsObjectOnScreen(x, y, width, height)) return;


            sprite.Transform = Matrix.Scaling(1f, 1f, 1f);
            /*
            if (width == 0 || height == 0)
            {
                if (width == 0) width = pixel_width;
                else width = (int)(width * zoom);
                if (height == 0) height = pixel_width;
                else height = (int)(height * zoom);
                DrawTexture(tx, new Rectangle(0, 0, width, height), new Vector3(0, 0, 0), new Vector3((int)((x - (int)(screen.X / zoom)) * zoom), (int)((y - (int)(screen.Y / zoom)) * zoom), 0), color);
            }
            else
            {*/
                DrawLinePBPDoted(X1, Y1, X2, Y2, color, color2, color3, color4);
            //}
            sprite.Transform = Matrix.Scaling((float)zoom, (float)zoom, 1f);
        }

        public void DrawArrow(int x0, int y0, int x1, int y1, Color color)
        {
            int x2, y2, x3, y3;

            double angle = Math.Atan2(y1 - y0, x1 - x0) + Math.PI;

            x2 = (int)(x1 + 10 * Math.Cos(angle - Math.PI / 8));
            y2 = (int)(y1 + 10 * Math.Sin(angle - Math.PI / 8));
            x3 = (int)(x1 + 10 * Math.Cos(angle + Math.PI / 8));
            y3 = (int)(y1 + 10 * Math.Sin(angle + Math.PI / 8));

            DrawLine(x1, y1, x0, y0, color);
            DrawLine(x1, y1, x2, y2, color);
            DrawLine(x1, y1, x3, y3, color);
        }

        public void DrawBézierSplineCubic(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, Color color)
        {
            for (double i = 0; i < 1; i += 0.01)
            {
                // The Green Lines
                int xa = getPt(x1, x2, i);
                int ya = getPt(y1, y2, i);
                int xb = getPt(x2, x3, i);
                int yb = getPt(y2, y3, i);
                int xc = getPt(x3, x4, i);
                int yc = getPt(y3, y4, i);

                // The Blue Line
                int xm = getPt(xa, xb, i);
                int ym = getPt(ya, yb, i);
                int xn = getPt(xb, xc, i);
                int yn = getPt(yb, yc, i);

                // The Black Dot
                int x = getPt(xm, xn, i);
                int y = getPt(ym, yn, i);

                DrawLinePBP(x, y, x, y, color);
            }
        }

        public void DrawBézierSplineQuadratic(int x1, int y1, int x2, int y2, int x3, int y3, Color color)
        {
            for (double i = 0; i < 1; i += 0.01)
            {
                // The Green Line
                int xa = getPt(x1, x2, i);
                int ya = getPt(y1, y2, i);
                int xb = getPt(x2, x3, i);
                int yb = getPt(y2, y3, i);

                // The Black Dot
                int x = getPt(xa, xb, i);
                int y = getPt(ya, yb, i);

                DrawLinePBP(x, y, x, y, color);
            }
        }



        int getPt(int n1, int n2, double perc)
        {
            int diff = n2 - n1;

            return (int)(n1 + (diff * perc));
        }

        void DrawLinePBP(int x0, int y0, int x1, int y1, Color color)
        {
            Rectangle screen = _parent.GetScreen();
            double zoom = _parent.GetZoom();
            int dx, dy, inx, iny, e;
            int pixel_width = (int)Math.Ceiling(zoom);

            dx = x1 - x0;
            dy = y1 - y0;
            inx = dx > 0 ? 1 : -1;
            iny = dy > 0 ? 1 : -1;

            dx = Math.Abs(dx);
            dy = Math.Abs(dy);

            if (dx >= dy)
            {
                dy <<= 1;
                e = dy - dx;
                dx <<= 1;
                while (x0 != x1)
                {
                    DrawTexture(tx, new Rectangle(0, 0, pixel_width, pixel_width), new Vector3(0, 0, 0), new Vector3((int)((x0 - (int)(screen.X / zoom)) * zoom), (int)((y0 - (int)(screen.Y / zoom)) * zoom), 0), color);
                    if (e >= 0)
                    {
                        y0 += iny;
                        e -= dx;
                    }
                    e += dy; x0 += inx;
                }
            }
            else
            {
                dx <<= 1;
                e = dx - dy;
                dy <<= 1;
                while (y0 != y1)
                {
                    DrawTexture(tx, new Rectangle(0, 0, pixel_width, pixel_width), new Vector3(0, 0, 0), new Vector3((int)((x0 - (int)(screen.X / zoom)) * zoom), (int)((y0 - (int)(screen.Y / zoom)) * zoom), 0), color);
                    if (e >= 0)
                    {
                        x0 += inx;
                        e -= dy;
                    }
                    e += dx; y0 += iny;
                }
            }
            DrawTexture(tx, new Rectangle(0, 0, pixel_width, pixel_width), new Vector3(0, 0, 0), new Vector3((int)((x0 - (int)(screen.X / zoom)) * zoom), (int)((y0 - (int)(screen.Y / zoom)) * zoom), 0), color);
        }

        void DrawLinePBPDoted(int x0, int y0, int x1, int y1, Color color, Color color2, Color color3, Color color4)
        {
            Rectangle screen = _parent.GetScreen();
            double zoom = _parent.GetZoom();
            int dx, dy, inx, iny, e;
            int pixel_width = (int)(Math.Ceiling(zoom + 0.3));

            dx = x1 - x0;
            dy = y1 - y0;
            inx = dx > 0 ? 1 : -1;
            iny = dy > 0 ? 1 : -1;

            dx = Math.Abs(dx);
            dy = Math.Abs(dy);

            Color currentColor = color;
            int iterations = 0;

            if (dx >= dy)
            {
                dy <<= 1;
                e = dy - dx;
                dx <<= 1;
                while (x0 != x1)
                {
                    if (iterations >= 5)
                    {
                        if (currentColor == color4) currentColor = color;
                        else if (currentColor == color3) currentColor = color4;
                        else if (currentColor == color2) currentColor = color3;
                        else if (currentColor == color) currentColor = color2;

                        iterations = 0;
                    }


                    DrawTexture(tx, new Rectangle(0, 0, pixel_width, pixel_width), new Vector3(0, 0, 0), new Vector3((int)((x0 - (int)(screen.X / zoom)) * zoom), (int)((y0 - (int)(screen.Y / zoom)) * zoom), 0), currentColor);
                    if (e >= 0)
                    {
                        y0 += iny;
                        e -= dx;
                    }
                    e += dy; x0 += inx; iterations++;

                }
            }
            else
            {
                dx <<= 1;
                e = dx - dy;
                dy <<= 1;
                while (y0 != y1)
                {
                    if (iterations >= 5)
                    {
                        if (currentColor == color4) currentColor = color;
                        else if (currentColor == color3) currentColor = color4;
                        else if (currentColor == color2) currentColor = color3;
                        else if (currentColor == color) currentColor = color2;

                        iterations = 0;
                    }

                    DrawTexture(tx, new Rectangle(0, 0, pixel_width, pixel_width), new Vector3(0, 0, 0), new Vector3((int)((x0 - (int)(screen.X / zoom)) * zoom), (int)((y0 - (int)(screen.Y / zoom)) * zoom), 0), currentColor);
                    if (e >= 0)
                    {
                        x0 += inx;
                        e -= dy;
                    }
                    e += dx; y0 += iny; iterations++;
                }
            }
            DrawTexture(tx, new Rectangle(0, 0, pixel_width, pixel_width), new Vector3(0, 0, 0), new Vector3((int)((x0 - (int)(screen.X / zoom)) * zoom), (int)((y0 - (int)(screen.Y / zoom)) * zoom), 0), color);
        }

        public void DrawRectangle(int x1, int y1, int x2, int y2, Color color)
        {
            if (!IsObjectOnScreen(x1, y1, x2 - x1, y2 - y1)) return;
            Rectangle screen = _parent.GetScreen();
            double zoom = _parent.GetZoom();

            DrawTexture(tx, new Rectangle(0, 0, x2 - x1, y2 - y1), new Vector3(0, 0, 0), new Vector3(x1 - (int)(screen.X / zoom), y1 - (int)(screen.Y / zoom), 0), color);
        }

		public void DrawText(string text, int x, int y, int width, Color color, bool bold)
        {
            Rectangle screen = _parent.GetScreen();
            double zoom = _parent.GetZoom();
            if (width >= 30)
            {
                ((bold) ? fontBold : font).DrawText(sprite, text, new SharpDX.Rectangle(x - (int)(screen.X / zoom), y - (int)(screen.Y / zoom), width, 1000), FontDrawFlags.WordBreak, new SharpDX.Color(color.R, color.G, color.B, color.A));
            }
            else
            {
                ((bold) ? fontBold : font).DrawText(sprite, text, x - (int)(screen.X / zoom), y - (int)(screen.Y / zoom), new SharpDX.Color(color.R, color.G, color.B, color.A));
            }
        }
        public void DrawTextSmall(string text, int x, int y, int width, Color color, bool bold)
        {
			Rectangle screen = _parent.GetScreen();
            double zoom = _parent.GetZoom();
            sprite.Transform = Matrix.Scaling((float)zoom / 4, (float)zoom / 4, 1f);
            if (width >= 10)
            {
                ((bold) ? fontBold : font).DrawText(sprite, text, new SharpDX.Rectangle((x - (int)(screen.X / zoom)) * 4, (y - (int)(screen.Y / zoom)) * 4, width * 4, 1000), FontDrawFlags.WordBreak, new SharpDX.Color(color.R, color.G, color.B, color.A));
            }
            else
            {
                ((bold) ? fontBold : font).DrawText(sprite, text, (x - (int)(screen.X / zoom)) * 4, (y - (int)(screen.Y / zoom)) * 4, new SharpDX.Color(color.R, color.G, color.B, color.A));
            }
            sprite.Transform = Matrix.Scaling((float)zoom, (float)zoom, 1f);
        }
		public void Draw2DCursor(int x, int y)
		{
			DrawTexture2(hvcursor, new Rectangle(Point.Empty, Cursors.NoMove2D.Size), new Vector3(0, 0, 0), new Vector3(x - 16, y - 15, 0), Color.White);

		}
		public void DrawHorizCursor(int x, int y)
		{
			//hcursor = new Texture(_device, hcursorb, Usage.Dynamic, Pool.Default);
			DrawTexture2(hcursor, new Rectangle(Point.Empty, Cursors.NoMove2D.Size), new Vector3(0, 0, 0), new Vector3(x - 16, y - 15, 0), Color.White);
		}

		public void DrawVertCursor(int x, int y)
		{
			//vcursor = new Texture(_device, vcursorb, Usage.Dynamic, Pool.Default);
			DrawTexture2(vcursor, new Rectangle(Point.Empty, Cursors.NoMove2D.Size), new Vector3(0, 0, 0), new Vector3(x - 16, y - 15, 0), Color.White);
			DrawTexture2(hcursor, new Rectangle(Point.Empty, Cursors.NoMove2D.Size), new Vector3(0, 0, 0), new Vector3(x - 16, y - 15, 0), Color.White);
		}
		public void OnMouseMoveEventCreate()
        {
           Cursor.Position = new Point(Cursor.Position.X, Cursor.Position.Y);
		}

        public void DisposeDeviceResources(int type = 0)
        {
            if (tx != null)
            {
                tx.Dispose();
                tx = null;
            }

            if (font != null)
            {
                font.Dispose();
                font = null;
            }
            if (fontBold != null)
            {
                fontBold.Dispose();
                fontBold = null;
            }

            if (sprite != null || type == 1)
            {
                sprite.Dispose();
                sprite = null;
            }
            if (HUD != null || type == 1)
            {
                HUD.Dispose();
                HUD = null;
            }

            if (_parent != null && type == 1)
            {
                _parent.DisposeTextures();
                _parent = null;
            }
            if (_device != null && type == 1)
            {
                _device.Dispose();
                _device = null;
            }
            if (direct3d != null && type == 1)
            {
                direct3d.Dispose();
                direct3d = null;
            }
            
        }

        public new void Dispose()
        {
            DisposeDeviceResources();
            _parent.DisposeTextures();
            _device.Dispose();
            direct3d.Dispose();
            base.Dispose();
        }


    }
}
