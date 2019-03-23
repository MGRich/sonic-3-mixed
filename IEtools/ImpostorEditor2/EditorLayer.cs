﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using RSDKv5;
using SharpDX.Direct3D9;
using ImpostorEditor2.Actions;
using System.Windows.Forms;
using System.ComponentModel;
using System.Drawing.Design;

namespace ImpostorEditor2 {
    class EditorLayer : IDrawable, IDisposable {
        public SceneLayer Layer;

        const int TILES_CHUNK_SIZE = 16;

        public const int TILE_SIZE = 16;

        Texture[][] TileChunksTextures;

        public PointsMap SelectedTiles;
        public Dictionary<Point, ushort> SelectedTilesValue = new Dictionary<Point, ushort>();

        public PointsMap TempSelectionTiles;
        bool TempSelectionDeselect;

        bool FirstDrag;
        bool isDragOver;

        public bool Visible = true;

        [Category("General")]
        [DisplayName("Name")]
        [Description("Name of the selected layer.")]
        public string _Name { get { return Layer.Name; } set { Layer.Name = value; } }

        [Category("General")]
        [DisplayName("Width")]
        [Description("Layer width in tiles.")]
        public ushort _Width {
            get {
                return Layer.Width;
            }
            set {
                ushort[][] Tiles = new ushort[Layer.Height][];
                for (int i = 0; i < Layer.Height; ++i) {
                    Tiles[i] = new ushort[value];
                    for (int j = 0; j < value; ++j)
                        Tiles[i][j] = 0;
                    for (int j = 0; j < value && j < Layer.Width; ++j)
                        Tiles[i][j] = Layer.Tiles[i][j];
                }
                Layer.Tiles = Tiles;
                Layer.Width = value;
            }
        }

        [Category("General")]
        [DisplayName("Height")]
        [Description("Layer height in tiles.")]
        public ushort _Height {
            get {
                return Layer.Height;
            }
            set {
                ushort[][] Tiles = new ushort[value][];
                for (int i = 0; i < value; ++i) {
                    Tiles[i] = new ushort[Layer.Width];
                    for (int j = 0; j < Layer.Width; ++j)
                        Tiles[i][j] = 0;
                }
                for (int i = 0; i < value && i < Layer.Height; ++i) {
                    for (int j = 0; j < Layer.Width; ++j)
                        Tiles[i][j] = Layer.Tiles[i][j];
                }
                Layer.Tiles = Tiles;
                Layer.Height = value;
            }
        }

        [Category("General")]
        [DisplayName("Relative Y")]
        [Description("How much the layer moves in proportion to the camera vertically.")]
        public string _RelativeY { get { return Layer.RelativeY.ToString("X4"); } set { Layer.RelativeY = Convert.ToUInt16(value, 16); } }

        [Category("General")]
        [DisplayName("Constant Y")]
        [Description("How many pixels the layer moves vertically per frame.\n(80 for 1 pixel every 2 frames)")]
        public string _ConstantY { get { return Layer.ConstantY.ToString("X4"); } set { Layer.ConstantY = Convert.ToUInt16(value, 16); } }

        [Category("General")]
        [DisplayName("Infinite VScroll")]
        [Description("If true, the layer can infinitely scroll up/down. \n(Example: Marble Garden Zone Act 1)")]
        public bool _Looping { get { return Layer.IsScrollingVertical == 1; } set { Layer.IsScrollingVertical = (byte)(value ? 1 : 0); } }

        public ToolStripDropDownButton ToolStripButton = null;

        public List<IAction> Actions = new List<IAction>();

        static int DivideRoundUp(int number, int by) {
            return (number + by - 1) / by;
        }

        public class PointsMap {
            HashSet<Point>[][] PointsChunks;
            HashSet<Point> OutOfBoundsPoints = new HashSet<Point>();
            public int Count = 0;

            public PointsMap(int width, int height) {
                PointsChunks = new HashSet<Point>[DivideRoundUp(height, TILES_CHUNK_SIZE)][];
                for (int i = 0; i < PointsChunks.Length; ++i) {
                    PointsChunks[i] = new HashSet<Point>[DivideRoundUp(width, TILES_CHUNK_SIZE)];
                    for (int j = 0; j < PointsChunks[i].Length; ++j)
                        PointsChunks[i][j] = new HashSet<Point>();
                }
            }

            public void Add(Point point) {
                HashSet<Point> h;
                if (point.Y < 0 || point.X < 0 || point.Y / TILES_CHUNK_SIZE >= PointsChunks.Length || point.X / TILES_CHUNK_SIZE >= PointsChunks[0].Length)
                    h = OutOfBoundsPoints;
                else
                    h = PointsChunks[point.Y / TILES_CHUNK_SIZE][point.X / TILES_CHUNK_SIZE];
                Count -= h.Count;
                h.Add(point);
                Count += h.Count;
            }

            public void Remove(Point point) {
                HashSet<Point> h;
                if (point.Y < 0 || point.X < 0 || point.Y / TILES_CHUNK_SIZE >= PointsChunks.Length || point.X / TILES_CHUNK_SIZE >= PointsChunks[0].Length)
                    h = OutOfBoundsPoints;
                else
                    h = PointsChunks[point.Y / TILES_CHUNK_SIZE][point.X / TILES_CHUNK_SIZE];
                Count -= h.Count;
                h.Remove(point);
                Count += h.Count;
            }

            public bool Contains(Point point) {
                if (point.Y < 0 || point.X < 0 || point.Y / TILES_CHUNK_SIZE >= PointsChunks.Length || point.X / TILES_CHUNK_SIZE >= PointsChunks[0].Length)
                    return OutOfBoundsPoints.Contains(point);
                else
                    return PointsChunks[point.Y / TILES_CHUNK_SIZE][point.X / TILES_CHUNK_SIZE].Contains(point);
            }

            public bool IsChunkUsed(int x, int y) {
                return PointsChunks[y][x].Count > 0;
            }

            public void Clear() {
                for (int i = 0; i < PointsChunks.Length; ++i)
                    for (int j = 0; j < PointsChunks[i].Length; ++j)
                        PointsChunks[i][j].Clear();
                OutOfBoundsPoints.Clear();
                Count = 0;
            }

            public HashSet<Point> GetChunkPoint(int x, int y) {
                return PointsChunks[y][x];
            }

            public List<Point> PopAll() {
                List<Point> points = GetAll();
                Clear();
                return points;
            }

            public List<Point> GetAll() {
                List<Point> points = new List<Point>(Count);
                for (int i = 0; i < PointsChunks.Length; ++i)
                    for (int j = 0; j < PointsChunks[i].Length; ++j)
                        points.AddRange(PointsChunks[i][j]);
                points.AddRange(OutOfBoundsPoints);
                return points;
            }

            public void AddPoints(List<Point> points) {
                points.ForEach(point => Add(point));
            }


        }

        public EditorLayer(SceneLayer layer) {
            this.Layer = layer;

            TileChunksTextures = new Texture[DivideRoundUp(this.Layer.Height, TILES_CHUNK_SIZE)][];
            for (int i = 0; i < TileChunksTextures.Length; ++i)
                TileChunksTextures[i] = new Texture[DivideRoundUp(this.Layer.Width, TILES_CHUNK_SIZE)];

            SelectedTiles = new PointsMap(this.Layer.Width, this.Layer.Height);
            TempSelectionTiles = new PointsMap(this.Layer.Width, this.Layer.Height);
        }

        /*
         * int PS[] =    {  0x0040,   0x0008,   0x0008,   0x0005,   0x0005,   0x0006,   0x00F0,   0x0006,   0x0005,   0x0005,   0x0008,   0x0008,   0x0030,   0x80C0,   0x7FFF };
        float PAS[] = {  0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,     };
        float PM[] =  {  0.250f, 0.21875f,  0.1875f, 0.15625f,   0.125f, 0.09375f,  0.0625f, 0.09375f,   0.125f, 0.15625f,  0.1875f, 0.21875f,   0.250f,   0.250f,   0.125f };
         * 
         * */

        public void StartDrag() {
            FirstDrag = true;
        }

        public void StartDragOver(Point point, ushort value) {
            Deselect();
            isDragOver = true;
            DragOver(point, value);
        }

        public void DragOver(Point point, ushort value) {
            SelectedTiles.Clear();
            SelectedTilesValue.Clear();
            point = new Point(point.X / TILE_SIZE, point.Y / TILE_SIZE);
            SelectedTiles.Add(point);
            SelectedTilesValue[point] = value;
        }

        public void EndDragOver(bool remove) {
            if (isDragOver) {
                if (remove) {
                    SelectedTiles.Clear();
                    SelectedTilesValue.Clear();
                }
                isDragOver = false;
            }
        }

        private void DetachSelected() {
            foreach (Point point in SelectedTiles.GetAll()) {
                if (!SelectedTilesValue.ContainsKey(point)) {
                    // Not moved yet
                    SelectedTilesValue[point] = Layer.Tiles[point.Y][point.X];
                    RemoveTile(point);
                }
            }
        }

        public void MoveSelected(Point oldPos, Point newPos, bool duplicate) {
            oldPos = new Point(oldPos.X / TILE_SIZE, oldPos.Y / TILE_SIZE);
            newPos = new Point(newPos.X / TILE_SIZE, newPos.Y / TILE_SIZE);
            if (oldPos != newPos) {
                duplicate &= FirstDrag;
                FirstDrag = false;
                Dictionary<Point, ushort> newDict = new Dictionary<Point, ushort>();
                List<Point> newPoints = new List<Point>(SelectedTiles.Count);
                foreach (Point point in SelectedTiles.PopAll()) {
                    Point newPoint = new Point(point.X + (newPos.X - oldPos.X), point.Y + (newPos.Y - oldPos.Y));
                    newPoints.Add(newPoint);
                    if (SelectedTilesValue.ContainsKey(point)) {
                        newDict[newPoint] = SelectedTilesValue[point];
                    }
                    else {
                        // Not moved yet
                        newDict[newPoint] = Layer.Tiles[point.Y][point.X];
                        if (!duplicate) RemoveTile(point);
                    }
                }
                if (duplicate) {
                    Deselect();
                    // Create new actions group
                    Actions.Add(new ActionDummy());
                }
                SelectedTilesValue = newDict;
                SelectedTiles.AddPoints(newPoints);
            }
        }

        public void MoveSelectedQuonta(Point change) {
            MoveSelected(Point.Empty, new Point(change.X * TILE_SIZE, change.Y * TILE_SIZE), false);
        }

        public void DeleteSelected() {
            bool removedSomething = SelectedTiles.Count > 0;
            foreach (Point p in SelectedTiles.PopAll()) {
                // Remove only tiles that not moved, because we already removed the moved tiles
                if (!SelectedTilesValue.ContainsKey(p)) {
                    RemoveTile(p);
                }
            }
            if (removedSomething)
                Actions.Add(new ActionsGroupCloseMarker());
            SelectedTilesValue.Clear();
        }

        public void FlipPropertySelected(ushort bit) {
            DetachSelected();

            List<Point> points = new List<Point>(SelectedTilesValue.Keys);
            foreach (Point point in points) {
                SelectedTilesValue[point] ^= bit;
            }
        }

        public void SetPropertySelected(int bit, bool state) {
            DetachSelected();

            List<Point> points = new List<Point>(SelectedTilesValue.Keys);
            foreach (Point point in points) {
                if (state)
                    SelectedTilesValue[point] |= (ushort)(1 << bit);
                else
                    SelectedTilesValue[point] &= (ushort)(~(1 << bit));
            }
        }

        public List<ushort> GetSelectedValues() {
            // Including temp selection
            List<ushort> selectedValues = new List<ushort>();
            foreach (Point point in SelectedTiles.GetAll()) {
                if (TempSelectionDeselect && TempSelectionTiles.Contains(point)) continue;
                if (SelectedTilesValue.ContainsKey(point)) {
                    selectedValues.Add(SelectedTilesValue[point]);
                }
                else {
                    // Not moved yet
                    selectedValues.Add(Layer.Tiles[point.Y][point.X]);
                }
            }
            foreach (Point point in TempSelectionTiles.GetAll()) {
                if (SelectedTiles.Contains(point)) continue;
                selectedValues.Add(Layer.Tiles[point.Y][point.X]);
            }
            return selectedValues;
        }

        public Dictionary<Point, ushort> CopyToClipboard(bool keepPosition = false) {
            if (SelectedTiles.Count == 0) return null;
            int minX = 0, minY = 0;

            Dictionary<Point, ushort> copiedTiles = new Dictionary<Point, ushort>(SelectedTilesValue); ;
            foreach (Point point in SelectedTiles.GetAll()) {
                if (!copiedTiles.ContainsKey(point)) {
                    // Not moved yet
                    copiedTiles[point] = GetTile(point);
                }
            }
            if (!keepPosition) {
                minX = copiedTiles.Keys.Min(x => x.X);
                minY = copiedTiles.Keys.Min(x => x.Y);
            }
            return copiedTiles.ToDictionary(x => new Point(x.Key.X - minX, x.Key.Y - minY), x => x.Value);
        }

        public void PasteFromClipboard(Point newPos, Dictionary<Point, ushort> points) {
            newPos = new Point(newPos.X / TILE_SIZE, newPos.Y / TILE_SIZE);
            Deselect();
            foreach (KeyValuePair<Point, ushort> point in points) {
                Point tilePos = new Point(point.Key.X + newPos.X, point.Key.Y + newPos.Y);
                SelectedTiles.Add(tilePos);
                SelectedTilesValue[tilePos] = point.Value;
            }
            // Create new actions group
            Actions.Add(new ActionDummy());
        }

        public void Select(Rectangle area, bool addSelection = false, bool deselectIfSelected = false) {
            if (!addSelection) Deselect();
            for (int y = Math.Max(area.Y / TILE_SIZE, 0); y < Math.Min(DivideRoundUp(area.Y + area.Height, TILE_SIZE), Layer.Height); ++y) {
                for (int x = Math.Max(area.X / TILE_SIZE, 0); x < Math.Min(DivideRoundUp(area.X + area.Width, TILE_SIZE), Layer.Width); ++x) {
                    if (addSelection || deselectIfSelected) {
                        Point p = new Point(x, y);
                        if (SelectedTiles.Contains(p)) {
                            if (deselectIfSelected) {
                                // Deselect
                                DeselectPoint(p);
                            }
                            // Don't add already selected tile, or if it was just deslected
                            continue;
                        }
                    }
                    if (Layer.Tiles[y][x] != 0xffff) {
                        SelectedTiles.Add(new Point(x, y));
                    }
                }
            }
        }

        public void Select(Point point, bool addSelection = false, bool deselectIfSelected = false) {
            if (!addSelection) Deselect();
            point = new Point(point.X / TILE_SIZE, point.Y / TILE_SIZE);
            if (point.X >= 0 && point.Y >= 0 && point.X < this.Layer.Tiles[0].Length && point.Y < this.Layer.Tiles.Length) {
                if (deselectIfSelected && SelectedTiles.Contains(point)) {
                    // Deselect
                    DeselectPoint(point);
                }
                else if (this.Layer.Tiles[point.Y][point.X] != 0xffff) {
                    // Just add the point
                    SelectedTiles.Add(point);
                }
            }
        }

        public void TempSelection(Rectangle area, bool deselectIfSelected) {
            TempSelectionTiles.Clear();
            TempSelectionDeselect = deselectIfSelected;
            for (int y = Math.Max(area.Y / TILE_SIZE, 0); y < Math.Min(DivideRoundUp(area.Y + area.Height, TILE_SIZE), Layer.Height); ++y) {
                for (int x = Math.Max(area.X / TILE_SIZE, 0); x < Math.Min(DivideRoundUp(area.X + area.Width, TILE_SIZE), Layer.Width); ++x) {
                    if (SelectedTiles.Contains(new Point(x, y)) || Layer.Tiles[y][x] != 0xffff) {
                        TempSelectionTiles.Add(new Point(x, y));
                    }
                }
            }
        }

        public void EndTempSelection() {
            TempSelectionTiles.Clear();
        }

        private void InvalidateChunk(int x, int y) {
            TileChunksTextures[y][x]?.Dispose();
            TileChunksTextures[y][x] = null;
        }

        private ushort GetTile(Point point) {
            return Layer.Tiles[point.Y][point.X];
        }

        private void SetTile(Point point, ushort value, bool addAction = true) {
            if (addAction)
                Actions.Add(new ActionChangeTile((x, y) => SetTile(x, y, false), point, Layer.Tiles[point.Y][point.X], value));
            Layer.Tiles[point.Y][point.X] = value;
            InvalidateChunk(point.X / TILES_CHUNK_SIZE, point.Y / TILES_CHUNK_SIZE);
        }

        private void RemoveTile(Point point) {
            SetTile(point, 0x0);
        }

        private void DeselectPoint(Point p) {
            if (SelectedTilesValue.ContainsKey(p)) {
                // Or else it wasn't moved at all
                SetTile(p, SelectedTilesValue[p]);
                SelectedTilesValue.Remove(p);
            }
            SelectedTiles.Remove(p);
        }

        public void Deselect() {
            bool hasTiles = SelectedTilesValue.Count > 0;
            foreach (KeyValuePair<Point, ushort> point in SelectedTilesValue) {
                // ignore out of bounds
                if (point.Key.X < 0 || point.Key.Y < 0 || point.Key.Y >= Layer.Height || point.Key.X >= Layer.Width) continue;
                SetTile(point.Key, point.Value);
            }
            if (hasTiles)
                Actions.Add(new ActionsGroupCloseMarker());

            SelectedTiles.Clear();
            SelectedTilesValue.Clear();
        }

        public bool IsPointSelected(Point point) {
            return SelectedTiles.Contains(new Point(point.X / TILE_SIZE, point.Y / TILE_SIZE));
        }

        public bool HasTileAt(Point point) {
            point = new Point(point.X / TILE_SIZE, point.Y / TILE_SIZE);
            if (point.X >= 0 && point.Y >= 0 && point.X < this.Layer.Tiles[0].Length && point.Y < this.Layer.Tiles.Length) {
                return Layer.Tiles[point.Y][point.X] != 0xffff;
            }
            return false;
        }

        public ushort GetTileAt(Point point) {
            point = new Point(point.X / TILE_SIZE, point.Y / TILE_SIZE);
            if (point.X >= 0 && point.Y >= 0 && point.X < this.Layer.Tiles[0].Length && point.Y < this.Layer.Tiles.Length) {
                if (SelectedTilesValue.ContainsKey(point)) return SelectedTilesValue[point];
                else return Layer.Tiles[point.Y][point.X];
            }
            return 0x0;
        }

        private Rectangle GetTilesChunkArea(int x, int y) {
            int y_start = y * TILES_CHUNK_SIZE;
            int y_end = Math.Min((y + 1) * TILES_CHUNK_SIZE, Layer.Height);

            int x_start = x * TILES_CHUNK_SIZE;
            int x_end = Math.Min((x + 1) * TILES_CHUNK_SIZE, Layer.Width);

            return new Rectangle(x_start, y_start, x_end - x_start, y_end - y_start);
        }

        public void DrawTile(Graphics g, ushort tile, int x, int y) {
            bool flipX = ((tile >> 10) & 1) == 1;
            bool flipY = ((tile >> 11) & 1) == 1;

            int tsx = 0;
            int tsy = (tile & 0x3ff);
            if (Editor.Instance.StageTiles.Image.W > 16) {
                tsx = (tile & 0x3ff) & 0x1F;
                tsy = (tile & 0x3ff) >> 5;
            }
            g.DrawImage(Editor.Instance.StageTiles.Image.GetBitmap(new Rectangle(tsx * TILE_SIZE, tsy * TILE_SIZE, TILE_SIZE, TILE_SIZE), flipX, flipY),
                new Rectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE));
        }

        public void DrawTile(DevicePanel d, ushort tile, int x, int y, bool selected, int Transperncy) {
            bool flipX = ((tile >> 10) & 1) == 1;
            bool flipY = ((tile >> 11) & 1) == 1;

            int tsx = 0;
            int tsy = (tile & 0x3ff);
            if (Editor.Instance.StageTiles.Image.W > 16) {
                tsx = (tile & 0x3ff) & 0x1F;
                tsy = (tile & 0x3ff) >> 5;
            }
            d.DrawBitmap(Editor.Instance.StageTiles.Image.GetTexture(d._device, new Rectangle(tsx * TILE_SIZE, tsy * TILE_SIZE, TILE_SIZE, TILE_SIZE), flipX, flipY),
                x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, selected, Transperncy);
            if (selected) {
                d.DrawLine(x * TILE_SIZE, y * TILE_SIZE, x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE, System.Drawing.Color.Brown);
                d.DrawLine(x * TILE_SIZE, y * TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE + TILE_SIZE, System.Drawing.Color.Brown);
                d.DrawLine(x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE, x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE, System.Drawing.Color.Brown);
                d.DrawLine(x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE + TILE_SIZE, System.Drawing.Color.Brown);
            }
        }

        public void Draw(Graphics g) {
            for (int y = 0; y < Layer.Height; ++y) {
                for (int x = 0; x < Layer.Width; ++x) {
                    if (this.Layer.Tiles[y][x] != 0xffff) {
                        DrawTile(g, Layer.Tiles[y][x], x, y);
                    }
                }
            }
        }

        private Texture GetTilesChunkTexture(DevicePanel d, int x, int y) {
            if (this.TileChunksTextures[y][x] != null) return this.TileChunksTextures[y][x];

            Rectangle rect = GetTilesChunkArea(x, y);

            using (Bitmap bmp = new Bitmap(rect.Width * TILE_SIZE, rect.Height * TILE_SIZE, System.Drawing.Imaging.PixelFormat.Format32bppArgb)) {
                using (Graphics g = Graphics.FromImage(bmp)) {
                    for (int ty = rect.Y; ty < rect.Y + rect.Height; ++ty) {
                        for (int tx = rect.X; tx < rect.X + rect.Width; ++tx) {
                            if (this.Layer.Tiles[ty][tx] != 0xffff) {
                                DrawTile(g, Layer.Tiles[ty][tx], tx - rect.X, ty - rect.Y);
                            }
                        }
                    }
                }
                this.TileChunksTextures[y][x] = TextureCreator.FromBitmap(d._device, bmp);
            }
            return this.TileChunksTextures[y][x];
        }

        private void DrawTilesChunk(DevicePanel d, int x, int y, int Transperncy) {
            Rectangle rect = GetTilesChunkArea(x, y);

            for (int ty = rect.Y; ty < rect.Y + rect.Height; ++ty) {
                for (int tx = rect.X; tx < rect.X + rect.Width; ++tx) {
                    Point p = new Point(tx, ty);
                    // We will draw those later
                    if (SelectedTiles.Contains(p) || TempSelectionTiles.Contains(p)) continue;

                    if (this.Layer.Tiles[ty][tx] != 0xffff) {
                        DrawTile(d, this.Layer.Tiles[ty][tx], tx, ty, false, Transperncy);
                    }
                }
            }
        }

        private void DrawSelectedTiles(DevicePanel d, int x, int y, int Transperncy) {
            foreach (Point p in SelectedTiles.GetChunkPoint(x, y))
                if (SelectedTilesValue.ContainsKey(p))
                    DrawTile(d, SelectedTilesValue[p], p.X, p.Y, !TempSelectionDeselect || !TempSelectionTiles.Contains(p), Transperncy);
                else // It is still in the original place
                    DrawTile(d, Layer.Tiles[p.Y][p.X], p.X, p.Y, !TempSelectionDeselect || !TempSelectionTiles.Contains(p), Transperncy);

            foreach (Point p in TempSelectionTiles.GetChunkPoint(x, y)) {
                if (SelectedTiles.Contains(p)) continue;
                DrawTile(d, Layer.Tiles[p.Y][p.X], p.X, p.Y, true, Transperncy);
            }
        }

        public void Draw(DevicePanel d) {
            if (!Visible) return;

            bool LoopX = true;

            bool DrawLined = Editor.Instance.EditLayer != this && Layer.ScrollIndexes.Length > 1;

            int Transperncy = (Editor.Instance.EditLayer != null && Editor.Instance.EditLayer != this) ? 0x32 : 0xFF;

            Rectangle screen = d.GetScreen();

            int alteredX = screen.X * Layer.ScrollingInfo[0].RelativeX / 0x100;
            int alteredY = screen.Y * Layer.RelativeY / 0x100;

            int start_x = alteredX / (TILES_CHUNK_SIZE * TILE_SIZE);
            int end_x = Math.Min(DivideRoundUp(alteredX + screen.Width, TILES_CHUNK_SIZE * TILE_SIZE), TileChunksTextures[0].Length);
            if (LoopX)
                end_x = DivideRoundUp(alteredX + screen.Width, TILES_CHUNK_SIZE * TILE_SIZE);

            int start_y = alteredY / (TILES_CHUNK_SIZE * TILE_SIZE);
            int end_y = Math.Min(DivideRoundUp(alteredY + screen.Height, TILES_CHUNK_SIZE * TILE_SIZE), TileChunksTextures.Length);


            for (int y = start_y; y < end_y; y++) {
                for (int oldx = start_x; oldx < end_x; oldx++) {
                    int x = oldx % TileChunksTextures[0].Length;

                    Rectangle rect = GetTilesChunkArea(x, y);

                    rect.X += oldx / TileChunksTextures[0].Length * TileChunksTextures[0].Length * TILES_CHUNK_SIZE;

                    if (SelectedTiles.IsChunkUsed(x, y) || TempSelectionTiles.IsChunkUsed(x, y)) {
                        // draw one by one
                        DrawTilesChunk(d, x, y, Transperncy);
                    }
                    else {
                        d.DrawBitmap(GetTilesChunkTexture(d, x, y),
                            rect.X * TILE_SIZE - screen.X * Layer.ScrollingInfo[0].RelativeX / 0x100 + screen.X,
                            rect.Y * TILE_SIZE - screen.Y * Layer.RelativeY / 0x100 + screen.Y,
                            rect.Width * TILE_SIZE,
                            rect.Height * TILE_SIZE,
                            false,
                            Transperncy);
                    }
                    DrawSelectedTiles(d, x, y, Transperncy);
                }
            }
        }

        public void Dispose() {
            foreach (Texture[] textures in TileChunksTextures)
                foreach (Texture texture in textures)
                    if (texture != null)
                        texture.Dispose();
            TileChunksTextures = null;
        }

        public void DisposeTextures() {
            foreach (Texture[] textures in TileChunksTextures) {
                for (int i = 0; i < textures.Length; ++i) {
                    if (textures[i] != null) {
                        textures[i].Dispose();
                        textures[i] = null;
                    }
                }
            }
        }
    }
}
