﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using RSDKv5;
using SharpDX.Direct3D9;
using ManiacEditor.Actions;
using System.ComponentModel;
using System.Diagnostics;
using System.Windows.Forms;

namespace ManiacEditor
{
    public class EditorLayer : IDrawable
    {
        private SceneLayer _layer;
        internal SceneLayer Layer { get => _layer; }

        public DevicePanel GraphicsPanel;

        public Editor EditorInstance;

        Texture[][] TileChunksTextures;

        Texture[] ChunksTextures;

        public PointsMap SelectedTiles;
        
        public Dictionary<Point, ushort> SelectedTilesValue = new Dictionary<Point, ushort>();

        public PointsMap TempSelectionTiles;
        public PointsMap TempSelectionDeselectTiles;
        bool TempSelectionDeselect;
        public int GlobalSelectedTiles;

        bool FirstDrag;
        bool isDragOver;

        public List<IAction> Actions = new List<IAction>();

        public string Name
        {
            get
            {
                
                string internalName = _layer.Name;
                return internalName?.TrimEnd('\0');
            }
            set
            {
                string name = value;
                if (name == null) name = "\0";
                if (!name.EndsWith("\0")) name += "\0";
                _layer.Name = name;
            }
        }

        public byte Behaviour
		{
            get => _layer.Behaviour;
            set => _layer.Behaviour = value;
        }

        public byte DrawingOrder
		{
            get => _layer.DrawingOrder;
            set => _layer.DrawingOrder = value;
        }

        public short RelativeSpeed
		{
            get => _layer.RelativeSpeed;
            set => _layer.RelativeSpeed = value;
        }

        public short ConstantSpeed
		{
            get => _layer.ConstantSpeed;
            set => _layer.ConstantSpeed = value;
        }

        public ushort Height { get => _layer.Height; }
        public ushort Width { get => _layer.Width; }
        public int HeightPixels { get => _layer.Height * EditorConstants.TILE_SIZE; }
        public int WidthPixels { get => _layer.Height * EditorConstants.TILE_SIZE; }

        /// <summary>
        /// Collection of rules and mappings representing the horizontal scrolling info
        /// and other rules affecting lines of pixels in this layer
        /// </summary>
        public IList<HorizontalLayerScroll> HorizontalLayerScroll { get => _horizontalLayerRules; }
        private IList<HorizontalLayerScroll> _horizontalLayerRules;

        static int DivideRoundUp(int number, int by)
        {
            return (number + by - 1) / by;
        }

        public class PointsMap
        {
            HashSet<Point>[][] PointsChunks;
            HashSet<Point> OutOfBoundsPoints = new HashSet<Point>();
            public int Count = 0;

            public PointsMap(int width, int height)
            {
                PointsChunks = new HashSet<Point>[DivideRoundUp(height, EditorConstants.TILES_CHUNK_SIZE)][];
                for (int i = 0; i < PointsChunks.Length; ++i)
                {
                    PointsChunks[i] = new HashSet<Point>[DivideRoundUp(width, EditorConstants.TILES_CHUNK_SIZE)];
                    for (int j = 0; j < PointsChunks[i].Length; ++j)
                        PointsChunks[i][j] = new HashSet<Point>();
                }
            }

            public void Add(Point point)
            {

                HashSet<Point> h;
                if (point.Y < 0 || point.X < 0 || point.Y / EditorConstants.TILES_CHUNK_SIZE >= PointsChunks.Length || point.X / EditorConstants.TILES_CHUNK_SIZE >= PointsChunks[0].Length)
                    h = OutOfBoundsPoints;
                else
                    h = PointsChunks[point.Y / EditorConstants.TILES_CHUNK_SIZE][point.X / EditorConstants.TILES_CHUNK_SIZE];
                Count -= h.Count;
                h.Add(point);
                Count += h.Count;
            }
            
            public void Remove(Point point)
            {
                HashSet<Point> h;
                if (point.Y < 0 || point.X < 0 || point.Y / EditorConstants.TILES_CHUNK_SIZE >= PointsChunks.Length || point.X / EditorConstants.TILES_CHUNK_SIZE >= PointsChunks[0].Length)
                    h = OutOfBoundsPoints;
                else
                    h = PointsChunks[point.Y / EditorConstants.TILES_CHUNK_SIZE][point.X / EditorConstants.TILES_CHUNK_SIZE];
                Count -= h.Count;
                h.Remove(point);
                Count += h.Count;
            }

            public bool Contains(Point point)
            {
                if (point.Y < 0 || point.X < 0 || point.Y / EditorConstants.TILES_CHUNK_SIZE >= PointsChunks.Length || point.X / EditorConstants.TILES_CHUNK_SIZE >= PointsChunks[0].Length)
                    return OutOfBoundsPoints.Contains(point);
                else
                    return PointsChunks[point.Y / EditorConstants.TILES_CHUNK_SIZE][point.X / EditorConstants.TILES_CHUNK_SIZE].Contains(point);
            }

            public bool IsChunkUsed(int x, int y)
            {
                return PointsChunks[y][x].Count > 0;
            }

            public void Clear()
            {
                for (int i = 0; i < PointsChunks.Length; ++i)
                    for (int j = 0; j < PointsChunks[i].Length; ++j)
                        PointsChunks[i][j].Clear();
                OutOfBoundsPoints.Clear();
                Count = 0;
            }

            public HashSet<Point> GetChunkPoint(int x, int y)
            {
                return PointsChunks[y][x];
            }

			public List<Point> PopAll()
            {
                List<Point> points = GetAll();
                Clear();
                return points;
            }

            public List<Point> GetAll()
            {
                List<Point> points = new List<Point>(Count);
                for (int i = 0; i < PointsChunks.Length; ++i)
                    for (int j = 0; j < PointsChunks[i].Length; ++j)
                        points.AddRange(PointsChunks[i][j]);
                points.AddRange(OutOfBoundsPoints);
                return points;
            }

            public void AddPoints(List<Point> points)
            {
                points.ForEach(point => Add(point));
            }


        }

        public EditorLayer(SceneLayer layer, Editor instance)
        {
            EditorInstance = instance;
            _layer = layer;

            ChunksTextures = new Texture[1028];

            TileChunksTextures = new Texture[DivideRoundUp(Height, EditorConstants.TILES_CHUNK_SIZE)][];
            for (int i = 0; i < TileChunksTextures.Length; ++i)
                TileChunksTextures[i] = new Texture[DivideRoundUp(Width, EditorConstants.TILES_CHUNK_SIZE)];

            SelectedTiles = new PointsMap(Width, Height);
            TempSelectionTiles = new PointsMap(Width, Height);
            TempSelectionDeselectTiles = new PointsMap(Width, Height);

            _horizontalLayerRules = ReadHorizontalLineRules();
        }

        /// <summary>
        /// Interpret the layer's set of horizontal scroll rules (ScrollInfo),
        /// and the line level map (ScrollIndexes) into that set of rules.
        /// </summary>
        /// <returns>List of HorizontalLayerScroll objects containing the scrolling rules</returns>
        private IList<HorizontalLayerScroll> ReadHorizontalLineRules()
        {
            var tempList = new List<HorizontalLayerScroll>();
            byte generatedId = 0;
            foreach (var scrollInfo in _layer.ScrollingInfo)
            {
                tempList.Add(new HorizontalLayerScroll(generatedId, scrollInfo));
                ++generatedId;
            }

            var ruleMapCount = _layer.ScrollIndexes.Count();
            int i = 0;
            while (i < ruleMapCount)
            {
                var currentValue = _layer.ScrollIndexes[i];
                var currentRule = _layer.ScrollingInfo[currentValue];
                var currentCount = 0;
                var start = i;
                while (   i < ruleMapCount 
                       && currentValue == _layer.ScrollIndexes[i])
                {
                    ++currentCount;
                    ++i;
                }

                tempList.First(hlr => hlr.Id == currentValue).AddMapping(start, currentCount);
            }

            return tempList;
        }

        /// <summary>
        /// Persist the contents of the HorizontalLayerRules collection,
        /// to the Layer's RSDKv5 backing objects and arrays.
        /// </summary>
        public void WriteHorizontalLineRules()
        {
            var newIndexes = new byte[_layer.ScrollIndexes.Length];
            _layer.ScrollingInfo = _horizontalLayerRules.Select(hlr => hlr.ScrollInfo).ToList();

            // the internal ID may now be inaccurate
            // we were only using it for display purposes anyway
            // generate some correct ones, and use those!
            byte newIndex = 0;
            foreach (var hlr in _horizontalLayerRules)
            {
                foreach (var lml in hlr.LinesMapList)
                {
                    var count = lml.LineCount;
                    int index = lml.StartIndex;
                    for (int i = 0; i < count; i++)
                    {
                        newIndexes[index + i] = newIndex;
                    }
                }
                ++newIndex;
            }

            _layer.ScrollIndexes = newIndexes;
        }

        /// <summary>
        /// Creates a new HorizontalLayerScroll object with backing ScrollInfo object.
        /// Adding it to the HorizontalLayerRules collection.
        /// </summary>
        public void ProduceHorizontalLayerScroll()
        {
            var id = (byte)(_horizontalLayerRules.Select(hlr => hlr.Id).Max() + 1);
            var info = new ScrollInfo();

            _layer.ScrollingInfo.Add(info);
            var hls = new HorizontalLayerScroll(id, info);
            _horizontalLayerRules.Add(hls);
        }

        public void StartDrag()
        {
            FirstDrag = true;
            RefreshTileCount();
        }

        public void StartDragOver(Point point, ushort value)
        {
            Deselect();
            isDragOver = true;
            DragOver(point, value);
            RefreshTileCount();
        }

        public void DragOver(Point point, ushort value)
        {
            SelectedTiles.Clear();
            SelectedTilesValue.Clear();
            point = new Point(point.X / EditorConstants.TILE_SIZE, point.Y / EditorConstants.TILE_SIZE);
            SelectedTiles.Add(point);
            SelectedTilesValue[point] = value;
            RefreshTileCount();
        }

        public void EndDragOver(bool remove)
        {
            if (isDragOver)
            {
                if (remove)
                {
                    SelectedTiles.Clear();
                    SelectedTilesValue.Clear();
                    RefreshTileCount();
                }
                isDragOver = false;
                RefreshTileCount();
            }
        }

        private void DetachSelected()
        {
            foreach (Point point in SelectedTiles.GetAll())
            {
                if (!SelectedTilesValue.ContainsKey(point))
                {
                    // Not moved yet
                    SelectedTilesValue[point] = _layer.Tiles[point.Y][point.X];
                    RemoveTile(point);
                    RefreshTileCount();
                }
            }
        }

        public void MoveSelected(Point oldPos, Point newPos, bool duplicate)
        {
            //if (Properties.Settings.Default.AllowMoreRenderUpdates == true) EditorInstance.UpdateRender();
            oldPos = new Point(oldPos.X / EditorConstants.TILE_SIZE, oldPos.Y / EditorConstants.TILE_SIZE);
            newPos = new Point(newPos.X / EditorConstants.TILE_SIZE, newPos.Y / EditorConstants.TILE_SIZE);
            if (oldPos != newPos)
            {
                duplicate &= FirstDrag;
                FirstDrag = false;
                Dictionary<Point, ushort> newDict = new Dictionary<Point, ushort>();
                List<Point> newPoints = new List<Point>(SelectedTiles.Count);
                foreach (Point point in SelectedTiles.PopAll())
                {
                    Point newPoint = new Point(point.X + (newPos.X - oldPos.X), point.Y + (newPos.Y - oldPos.Y));
                    newPoints.Add(newPoint);
                    if (SelectedTilesValue.ContainsKey(point))
                    {
                        newDict[newPoint] = SelectedTilesValue[point];
                    }
                    else
                    {
                        // Not moved yet
                        newDict[newPoint] = _layer.Tiles[point.Y][point.X];
                        if (!duplicate) RemoveTile(point);
                    }
                }
                if (duplicate)
                {
                    Deselect();
                    // Create new actions group
                    Actions.Add(new ActionDummy());
                    RefreshTileCount();
                }
                SelectedTilesValue = newDict;
                SelectedTiles.AddPoints(newPoints);
                RefreshTileCount();
            }
        }

		public Point GetChunkCoordinates(int x, int y)
		{
			Point ChunkCoordinate = new Point();
			if (x != 0) ChunkCoordinate.X = x / 128;
			else ChunkCoordinate.X = 0;
			if (y != 0) ChunkCoordinate.Y = y / 128;
			else ChunkCoordinate.Y = 0;

			return ChunkCoordinate;
		}

		public Point GetChunkCoordinates(double x, double y)
		{
			Point ChunkCoordinate = new Point();
			if (x != 0) ChunkCoordinate.X = (int)(x / 128);
			else ChunkCoordinate.X = 0;
			if (y != 0) ChunkCoordinate.Y = (int)(y / 128);
			else ChunkCoordinate.Y = 0;

			return ChunkCoordinate;
		}

		public void MoveSelectedQuonta(Point change)
        {
            MoveSelected(Point.Empty, new Point(change.X * EditorConstants.TILE_SIZE, change.Y * EditorConstants.TILE_SIZE), false);
        }

        public void DeleteSelected()
        {
            bool removedSomething = SelectedTiles.Count > 0;
            foreach (Point p in SelectedTiles.PopAll())
            {
                // Remove only tiles that not moved, because we already removed the moved tiles
                if (!SelectedTilesValue.ContainsKey(p))
                {
                    RemoveTile(p);
                }
            }
            if (removedSomething)
            {
                Actions.Add(new ActionsGroupCloseMarker());
            }

            SelectedTilesValue.Clear();
            RefreshTileCount();

        }

        public void FlipPropertySelected(FlipDirection direction, bool flipIndividually = false)
        {
            DetachSelected();
            List<Point> points = new List<Point>(SelectedTilesValue.Keys);

            if (points.Count == 0) return;

            if (points.Count == 1 || flipIndividually)
            {
                FlipIndividualTiles(direction, points);
                return;
            }

            IEnumerable<int> monoCoordinates;

            if (direction == FlipDirection.Horizontal)
            {
                monoCoordinates = points.Select(p => p.X);
            }
            else
            {
                monoCoordinates = points.Select(p => p.Y);
            }

            int min = monoCoordinates.Min();
            int max = monoCoordinates.Max();
            int diff = max - min;

            if (diff == 0)
            {
                FlipIndividualTiles(direction, points);
            }
            else
            {
                FlipGroupTiles(direction, points, min, max);
            }
        }

        private void FlipIndividualTiles(FlipDirection direction, IEnumerable<Point> points)
        {
            foreach (Point point in points)
            {
                SelectedTilesValue[point] ^= (ushort)direction;
            }
        }

        private void FlipGroupTiles(FlipDirection direction, IEnumerable<Point> points, int min, int max)
        {
            Dictionary<Point, ushort> workingTiles = new Dictionary<Point, ushort>();
            foreach (Point point in points)
            {
                ushort tileValue = SelectedTilesValue[point];
                Point newPoint; 

                if (direction == FlipDirection.Horizontal)
                {
                    int fromLeft = point.X - min;
                    int fromRight = max - point.X;

                    int newX = fromLeft < fromRight ? max - fromLeft : min + fromRight;
                    newPoint = new Point(newX, point.Y);
                }
                else
                {
                    int fromBottom = point.Y - min;
                    int fromTop = max - point.Y;

                    int newY = fromBottom < fromTop ? max - fromBottom : min + fromTop;
                    newPoint = new Point(point.X, newY);
                }

                workingTiles.Add(newPoint, tileValue ^= (ushort)direction);
            }

            SelectedTiles.Clear();
            SelectedTilesValue.Clear();
            SelectedTiles.AddPoints(workingTiles.Select(wt => wt.Key).ToList());
            SelectedTilesValue = workingTiles;
        }

        public void SetPropertySelected(int bit, bool state)
        {
            DetachSelected();

            List<Point> points = new List<Point>(SelectedTilesValue.Keys);
            foreach (Point point in points)
            {
                if (state)
                    SelectedTilesValue[point] |= (ushort)(1 << bit);
                else
                    SelectedTilesValue[point] &= (ushort)(~(1 << bit));
            }
        }

        public List<ushort> GetSelectedValues()
        {
            // Including temp selection
            List<ushort> selectedValues = new List<ushort>();
            foreach (Point point in SelectedTiles.GetAll())
            {
                if (TempSelectionDeselect && TempSelectionTiles.Contains(point)) continue;
                if (SelectedTilesValue.ContainsKey(point))
                {
                    selectedValues.Add(SelectedTilesValue[point]);
                }
                else
                {
                    // Not moved yet
                    selectedValues.Add(_layer.Tiles[point.Y][point.X]);
                }
            }
            foreach (Point point in TempSelectionTiles.GetAll())
            {
                if (SelectedTiles.Contains(point)) continue;
                selectedValues.Add(_layer.Tiles[point.Y][point.X]);
            }
            RefreshTileCount();
            return selectedValues;
        }

        public Dictionary<Point, ushort> CopyToClipboard(bool keepPosition = false)
        {
            if (SelectedTiles.Count == 0) return null;
            int minX = 0, minY = 0;

            Dictionary<Point, ushort> copiedTiles = new Dictionary<Point, ushort>(SelectedTilesValue);;
            foreach (Point point in SelectedTiles.GetAll())
            {
                if (!copiedTiles.ContainsKey(point))
                {
                    // Not moved yet
                    copiedTiles[point] = GetTile(point);
                }
            }
            if (!keepPosition)
            {
                minX = copiedTiles.Keys.Min(x => x.X);
                minY = copiedTiles.Keys.Min(x => x.Y);
            }
            return copiedTiles.ToDictionary(x => new Point(x.Key.X - minX, x.Key.Y - minY), x => x.Value);
        }

		public static Tuple<Dictionary<Point, ushort>, Dictionary<Point, ushort>> CopyMultiSelectionToClipboard(EditorLayer layer1, EditorLayer layer2, bool keepPosition = false)
		{
			if (layer1.SelectedTiles.Count == 0 && layer2.SelectedTiles.Count == 0) return null;
			int minX = 0, minY = 0;

			Dictionary<Point, ushort> copiedTilesA = new Dictionary<Point, ushort>(layer1.SelectedTilesValue);
			Dictionary<Point, ushort> copiedTilesB = new Dictionary<Point, ushort>(layer2.SelectedTilesValue);
			foreach (Point point in layer1.SelectedTiles.GetAll())
			{
				if (!copiedTilesA.ContainsKey(point))
				{
					// Not moved yet
					copiedTilesA[point] = layer1.GetTile(point);
				}
			}
			foreach (Point point in layer2.SelectedTiles.GetAll())
			{
				if (!copiedTilesB.ContainsKey(point))
				{
					// Not moved yet
					copiedTilesB[point] = layer2.GetTile(point);
				}
			}
			if (!keepPosition)
			{
				int minX_A = (copiedTilesA.Count != 0 ? copiedTilesA.Keys.Min(x => x.X) : 0);
				int minY_A = (copiedTilesA.Count != 0 ? copiedTilesA.Keys.Min(x => x.Y) : 0);
                int minX_B = (copiedTilesB.Count != 0 ? copiedTilesB.Keys.Min(x => x.X) : 0);
                int minY_B = (copiedTilesB.Count != 0 ? copiedTilesB.Keys.Min(x => x.Y) : 0);
                minX = Math.Min(minX_A, minX_B);
				minY = Math.Min(minY_A, minY_B);
			}
			copiedTilesA = copiedTilesA.ToDictionary(x => new Point(x.Key.X - minX, x.Key.Y - minY), x => x.Value);
			copiedTilesB = copiedTilesB.ToDictionary(x => new Point(x.Key.X - minX, x.Key.Y - minY), x => x.Value);
			return new Tuple<Dictionary<Point, ushort>, Dictionary<Point, ushort>>(copiedTilesA, copiedTilesB);
		}

		public void PasteFromClipboard(Point newPos, Dictionary<Point, ushort> points)
        {
            try
            {
                newPos = new Point(newPos.X / EditorConstants.TILE_SIZE, newPos.Y / EditorConstants.TILE_SIZE);
                Deselect();
                foreach (KeyValuePair<Point, ushort> point in points)
                {
                    Point tilePos = new Point(point.Key.X + newPos.X, point.Key.Y + newPos.Y);
                    SelectedTiles.Add(tilePos);
                    SelectedTilesValue[tilePos] = point.Value;
                }
                // Create new actions group
                Actions.Add(new ActionDummy());
                RefreshTileCount();
            }
            catch
            {
                MessageBox.Show("Clipboard Content Problem!");
            }

        }

        public void Select(Rectangle area, bool addSelection = false, bool deselectIfSelected = false)
        {
            if (!addSelection) Deselect();
            for (int y = Math.Max(area.Y / EditorConstants.TILE_SIZE, 0); y < Math.Min(DivideRoundUp(area.Y + area.Height, EditorConstants.TILE_SIZE), _layer.Height); ++y)
            {
                for (int x = Math.Max(area.X / EditorConstants.TILE_SIZE, 0); x < Math.Min(DivideRoundUp(area.X + area.Width, EditorConstants.TILE_SIZE), _layer.Width); ++x)
                {
                    if (addSelection || deselectIfSelected)
                    {
                        Point p = new Point(x, y);
                        if (SelectedTiles.Contains(p))
                        {
                            if (deselectIfSelected)
                            {
                                // Deselect
                                DeselectPoint(p);
                                RefreshTileCount();
                            }
                            // Don't add already selected tile, or if it was just deslected
                            continue;
                        }
                    }
                    if (_layer.Tiles[y][x] != 0xffff)
                    {
                        SelectedTiles.Add(new Point(x, y));
                        RefreshTileCount();
                    }
					else if (_layer.Tiles[y][x] == 0xffff && EditorInstance.CopyAir)
					{ 
						SelectedTiles.Add(new Point(x, y));
						RefreshTileCount();
					}
				}
            }
        }

        public void Select(Point point, bool addSelection = false, bool deselectIfSelected = false)
        {
            if (!addSelection) Deselect();
            point = new Point(point.X / EditorConstants.TILE_SIZE, point.Y / EditorConstants.TILE_SIZE);
            EditorInstance.EditorState.SelectedTileX = point.X;
            EditorInstance.EditorState.SelectedTileY = point.Y;
            if (point.X >= 0 && point.Y >= 0 && point.X < this._layer.Tiles[0].Length && point.Y < this._layer.Tiles.Length)
            {
                if (deselectIfSelected && SelectedTiles.Contains(point))
                {
                    // Deselect
                    DeselectPoint(point);
                    RefreshTileCount();
                }
                else if (this._layer.Tiles[point.Y][point.X] != 0xffff || EditorInstance.CopyAir)
                {
                    // Just add the point
                    SelectedTiles.Add(point);
                    RefreshTileCount();
                }
            }
        }

        public void TempSelection(Rectangle area, bool deselectIfSelected)
        {
            TempSelectionTiles.Clear();
            TempSelectionDeselectTiles.Clear();
            TempSelectionDeselect = deselectIfSelected;
            for (int y = Math.Max(area.Y / EditorConstants.TILE_SIZE, 0); y < Math.Min(DivideRoundUp(area.Y + area.Height, EditorConstants.TILE_SIZE), _layer.Height); ++y)
            {
                for (int x = Math.Max(area.X / EditorConstants.TILE_SIZE, 0); x < Math.Min(DivideRoundUp(area.X + area.Width, EditorConstants.TILE_SIZE), _layer.Width); ++x)
                {
                    if (SelectedTiles.Contains(new Point(x, y)) || (_layer.Tiles[y][x] != 0xffff || EditorInstance.CopyAir))
                    {
                        TempSelectionTiles.Add(new Point(x, y));
                        if (SelectedTiles.Contains(new Point(x, y)) && TempSelectionTiles.Contains(new Point(x, y)))
                        {
                            TempSelectionDeselectTiles.Add(new Point(x, y));
                        }
                        RefreshTileCount();


                    }
                }
            }
        }

        public void EndTempSelection()
        {
            TempSelectionTiles.Clear();
            TempSelectionDeselectTiles.Clear();
            RefreshTileCount();
        }

        private void InvalidateChunk(int x, int y)
        {
            TileChunksTextures[y][x]?.Dispose();
            TileChunksTextures[y][x] = null;
        }

        private ushort GetTile(Point point)
        {
            return _layer.Tiles[point.Y][point.X];
            
        }

        private void SetTile(Point point, ushort value, bool addAction = true)
        {
            if (addAction)
                Actions.Add(new ActionChangeTile((x, y) => SetTile(x, y, false), point, _layer.Tiles[point.Y][point.X], value));
            _layer.Tiles[point.Y][point.X] = value;
            InvalidateChunk(point.X / EditorConstants.TILES_CHUNK_SIZE, point.Y / EditorConstants.TILES_CHUNK_SIZE);
        }

        private void RemoveTile(Point point)
        {
            SetTile(point, 0xffff);
            RefreshTileCount();
        }

        private void DeselectPoint(Point p)
        {
            if (SelectedTilesValue.ContainsKey(p))
            {
                // Or else it wasn't moved at all
                SetTile(p, SelectedTilesValue[p]);
                SelectedTilesValue.Remove(p);
            }
            SelectedTiles.Remove(p);
        }

        public void Deselect()
        {
            bool hasTiles = SelectedTilesValue.Count > 0;
            foreach (KeyValuePair<Point, ushort> point in SelectedTilesValue)
            {
                // ignore out of bounds
                if (point.Key.X < 0 || point.Key.Y < 0 || point.Key.Y >= _layer.Height || point.Key.X >= _layer.Width) continue;
                SetTile(point.Key, point.Value);
            }
            if (hasTiles)
                Actions.Add(new ActionsGroupCloseMarker());

            SelectedTiles.Clear();
            SelectedTilesValue.Clear();
            EditorInstance.EditorState.SelectedTilesCount = 0;
        }

        public bool IsPointSelected(Point point)
        {
            return SelectedTiles.Contains(new Point(point.X / EditorConstants.TILE_SIZE, point.Y / EditorConstants.TILE_SIZE));
        }

        public bool HasTileAt(Point point)
        {
            point = new Point(point.X / EditorConstants.TILE_SIZE, point.Y / EditorConstants.TILE_SIZE);
            if (point.X >= 0 && point.Y >= 0 && point.X < this._layer.Tiles[0].Length && point.Y < this._layer.Tiles.Length)
            {
                return (_layer.Tiles[point.Y][point.X] != 0xffff || EditorInstance.CopyAir);
            }
            return false;
        }

        public ushort GetTileAt(Point point)
        {
            point = new Point(point.X / EditorConstants.TILE_SIZE, point.Y / EditorConstants.TILE_SIZE);
            if (point.X >= 0 && point.Y >= 0 && point.X < this._layer.Tiles[0].Length && point.Y < this._layer.Tiles.Length)
            {
                if (SelectedTilesValue.ContainsKey(point)) return SelectedTilesValue[point];
                else return _layer.Tiles[point.Y][point.X];
            }
            return 0xffff;
        }

		public ushort GetTileAt(int x, int y)
		{
			Point point = new Point(x, y);
			if (point.X >= 0 && point.Y >= 0 && point.X < this._layer.Tiles[0].Length && point.Y < this._layer.Tiles.Length)
			{
				if (SelectedTilesValue.ContainsKey(point)) return SelectedTilesValue[point];
				else return _layer.Tiles[point.Y][point.X];
			}
			return 0xffff;
		}



		private Rectangle GetChunkArea(int x, int y)
        {
            return new Rectangle(x, y, 128, 128);
        }

        private Rectangle GetTilesChunkArea(int x, int y)
        {
            int y_start = y * EditorConstants.TILES_CHUNK_SIZE;
            int y_end = Math.Min((y + 1) * EditorConstants.TILES_CHUNK_SIZE, _layer.Height);

            int x_start = x * EditorConstants.TILES_CHUNK_SIZE;
            int x_end = Math.Min((x + 1) * EditorConstants.TILES_CHUNK_SIZE, _layer.Width);


            return new Rectangle(x_start, y_start, x_end - x_start, y_end - y_start);
        }



        public void DrawTile(DevicePanel d, ushort tile, int x, int y, bool selected, int Transperncy)
		{
			if (tile != 0xffff)
			{
				ushort TileIndex = (ushort)(tile & 0x3ff);
				int TileIndexInt = (int)TileIndex;
				bool flipX = ((tile >> 10) & 1) == 1;
				bool flipY = ((tile >> 11) & 1) == 1;
				bool SolidTopA = ((tile >> 12) & 1) == 1;
				bool SolidLrbA = ((tile >> 13) & 1) == 1;
				bool SolidTopB = ((tile >> 14) & 1) == 1;
				bool SolidLrbB = ((tile >> 15) & 1) == 1;

				d.DrawBitmap(EditorInstance.EditorTiles.StageTiles.Image.GetTexture(d._device, new Rectangle(0, (tile & 0x3ff) * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE), flipX, flipY),
				x * EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, selected, Transperncy);

				if (EditorInstance.UIModes.ShowFlippedTileHelper == true)
				{
					d.DrawBitmap(EditorInstance.EditorTiles.StageTiles.EditorImage.GetTexture(d._device, new Rectangle(0, 3 * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE), false, false),
					x * EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, selected, Transperncy);
				}

				if (EditorInstance.UIModes.ShowTileID == true)
				{
					d.DrawBitmap(EditorInstance.EditorTiles.StageTiles.IDImage.GetTexture(d._device, new Rectangle(0, (tile & 0x3ff) * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE), false, false),
					x * EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, selected, Transperncy);
				}
			}

            if (selected)
            {
                d.DrawLine(x * EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE, x * EditorConstants.TILE_SIZE + EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE, System.Drawing.Color.Brown);
                d.DrawLine(x * EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE, x * EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE + EditorConstants.TILE_SIZE, System.Drawing.Color.Brown);
                d.DrawLine(x * EditorConstants.TILE_SIZE + EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE + EditorConstants.TILE_SIZE, x * EditorConstants.TILE_SIZE + EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE, System.Drawing.Color.Brown);
                d.DrawLine(x * EditorConstants.TILE_SIZE + EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE + EditorConstants.TILE_SIZE, x * EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE + EditorConstants.TILE_SIZE, System.Drawing.Color.Brown);
            }
        }
        public void DrawTile(Graphics g, ushort tile, int x, int y)
        {
            ushort TileIndex = (ushort)(tile & 0x3ff);
            int TileIndexInt = (int)TileIndex;
            bool flipX = ((tile >> 10) & 1) == 1;
            bool flipY = ((tile >> 11) & 1) == 1;
            bool SolidTopA = ((tile >> 12) & 1) == 1;
            bool SolidLrbA = ((tile >> 13) & 1) == 1;
            bool SolidTopB = ((tile >> 14) & 1) == 1;
            bool SolidLrbB = ((tile >> 15) & 1) == 1;
          
            g.DrawImage(EditorInstance.EditorTiles.StageTiles.Image.GetBitmap(new Rectangle(0, TileIndex * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE), flipX, flipY),
                new Rectangle(x * EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE));
            if (EditorInstance.UIModes.ShowCollisionA == true)
            {
                if (SolidLrbA || SolidTopA)
                {
					//Get a bitmap of the collision
					Bitmap cm = EditorInstance.CollisionLayerA[TileIndex].Clone(new Rectangle(0, 0, 16, 16), System.Drawing.Imaging.PixelFormat.DontCare);

					System.Drawing.Color AllSolid = System.Drawing.Color.FromArgb((int)EditorInstance.collisionOpacitySlider.Value, EditorInstance.CollisionAllSolid.R, EditorInstance.CollisionAllSolid.G, EditorInstance.CollisionAllSolid.B);
					System.Drawing.Color LRDSolid = System.Drawing.Color.FromArgb((int)EditorInstance.collisionOpacitySlider.Value, EditorInstance.CollisionLRDSolid.R, EditorInstance.CollisionLRDSolid.G, EditorInstance.CollisionLRDSolid.B);
					System.Drawing.Color TopOnlySolid = System.Drawing.Color.FromArgb((int)EditorInstance.collisionOpacitySlider.Value, EditorInstance.CollisionTopOnlySolid.R, EditorInstance.CollisionTopOnlySolid.G, EditorInstance.CollisionTopOnlySolid.B);

					if (SolidTopA && SolidLrbA)
					{
						cm = Extensions.ChangeImageColor(cm, EditorInstance.CollisionAllSolid, AllSolid);
					}

					if (SolidTopA && !SolidLrbA)
					{
						cm = Extensions.ChangeImageColor(cm, EditorInstance.CollisionAllSolid, TopOnlySolid);
					}

					if (SolidLrbA && !SolidTopA)
					{
						cm = Extensions.ChangeImageColor(cm, EditorInstance.CollisionAllSolid, LRDSolid);
					}

					if (flipX) { cm.RotateFlip(RotateFlipType.RotateNoneFlipX); }

                    if (flipY) { cm.RotateFlip(RotateFlipType.RotateNoneFlipY); }

                    g.DrawImage(cm, new Rectangle(x * EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE));
                }
            }
            if (EditorInstance.UIModes.ShowCollisionB == true)
            {
                if (SolidLrbB || SolidTopB)
                {
                    //Get a bitmap of the collision
                    Bitmap cm = EditorInstance.CollisionLayerB[TileIndex].Clone(new Rectangle(0, 0, 16, 16), System.Drawing.Imaging.PixelFormat.DontCare);

					System.Drawing.Color AllSolid = System.Drawing.Color.FromArgb((int)EditorInstance.collisionOpacitySlider.Value, EditorInstance.CollisionAllSolid.R, EditorInstance.CollisionAllSolid.G, EditorInstance.CollisionAllSolid.B);
					System.Drawing.Color LRDSolid = System.Drawing.Color.FromArgb((int)EditorInstance.collisionOpacitySlider.Value, EditorInstance.CollisionLRDSolid.R, EditorInstance.CollisionLRDSolid.G, EditorInstance.CollisionLRDSolid.B);
					System.Drawing.Color TopOnlySolid = System.Drawing.Color.FromArgb((int)EditorInstance.collisionOpacitySlider.Value, EditorInstance.CollisionTopOnlySolid.R, EditorInstance.CollisionTopOnlySolid.G, EditorInstance.CollisionTopOnlySolid.B);

					if (SolidTopB && SolidLrbB)
					{
						cm = Extensions.ChangeImageColor(cm, EditorInstance.CollisionAllSolid, AllSolid);
					}

					if (SolidTopB && !SolidLrbB)
                    {
						cm = Extensions.ChangeImageColor(cm, EditorInstance.CollisionAllSolid, TopOnlySolid);
					}

                    if (SolidLrbB && !SolidTopB)
                    {
						cm = Extensions.ChangeImageColor(cm, EditorInstance.CollisionAllSolid, LRDSolid);
					}

					if (flipX) { cm.RotateFlip(RotateFlipType.RotateNoneFlipX); }

                    if (flipY) { cm.RotateFlip(RotateFlipType.RotateNoneFlipY); }

                    g.DrawImage(cm, new Rectangle(x * EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE));
                }
            }

            if (EditorInstance.UIModes.ShowFlippedTileHelper == true)
            {
                g.DrawImage(EditorInstance.EditorTiles.StageTiles.EditorImage.GetBitmap(new Rectangle(0, 3 * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE), false, false),
                            new Rectangle(x * EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE));
            }
            if (EditorInstance.UIModes.ShowTileID == true)
            {
                g.DrawImage(EditorInstance.EditorTiles.StageTiles.IDImage.GetBitmap(new Rectangle(0, TileIndex * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE), false, false),
                            new Rectangle(x * EditorConstants.TILE_SIZE, y * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE));
            }


        }

        public void DrawTile(Graphics g, ushort tile)
        {
            g.DrawImage(EditorInstance.EditorTiles.StageTiles.Image.GetBitmap(new Rectangle(0, 2 * EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE), false, false),
                new Rectangle(0, 0, EditorConstants.TILE_SIZE, EditorConstants.TILE_SIZE));
        }



        public void Draw(Graphics g)
        {
            for (int y = 0; y < _layer.Height; ++y)
            {
                for (int x = 0; x < _layer.Width; ++x)
                {
                    if (this._layer.Tiles[y][x] != 0xffff)
                    {
                        DrawTile(g, _layer.Tiles[y][x], x, y);
                    }
                }
            }
        }

        private Texture GetTilesChunkTexture(DevicePanel d, int x, int y)
        {
            if (this.TileChunksTextures[y][x] != null) return this.TileChunksTextures[y][x];

            Rectangle rect = GetTilesChunkArea(x, y);

            Bitmap bmp2 = new Bitmap(rect.Width * EditorConstants.TILE_SIZE, rect.Height * EditorConstants.TILE_SIZE, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            var squareSize = (bmp2.Width > bmp2.Height ? bmp2.Width : bmp2.Height);
            int factor = 32;
            int newSize = (int)Math.Round((squareSize / (double)factor), MidpointRounding.AwayFromZero) * factor;
            if (newSize == 0) newSize = factor;
            while (newSize < squareSize) newSize += factor;

            Bitmap bmp = new Bitmap(newSize, newSize, System.Drawing.Imaging.PixelFormat.Format32bppArgb);

            using (bmp)
            {
                using (Graphics g = Graphics.FromImage(bmp))
                {
                    for (int ty = rect.Y; ty < rect.Y + rect.Height; ++ty)
                    {
                        for (int tx = rect.X; tx < rect.X + rect.Width; ++tx)
                        {
                            if (this._layer.Tiles[ty][tx] != 0xffff)
                            {
                                DrawTile(g, _layer.Tiles[ty][tx], tx - rect.X, ty - rect.Y);
                                //DrawTile(g, _layer.Tiles[y][x], x, y);
                            }
                        }
                    }
                }
                this.TileChunksTextures[y][x] = TextureCreator.FromBitmap(d._device, bmp);
            }

            return this.TileChunksTextures[y][x];
        }

        public Texture GetChunkTexture(DevicePanel d, int x, int y)
        {
            if (this.ChunksTextures[x] != null) return this.ChunksTextures[x];

            Rectangle rect = GetChunkArea(x, y);

            Bitmap bmp2 = new Bitmap(rect.Width * EditorConstants.TILE_SIZE, rect.Height * EditorConstants.TILE_SIZE, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            var squareSize = (bmp2.Width > bmp2.Height ? bmp2.Width : bmp2.Height);
            int factor = 32;
            int newSize = (int)Math.Round((squareSize / (double)factor), MidpointRounding.AwayFromZero) * factor;
            if (newSize == 0) newSize = factor;
            while (newSize < squareSize) newSize += factor;

            Bitmap bmp = new Bitmap(newSize, newSize, System.Drawing.Imaging.PixelFormat.Format32bppArgb);

            using (bmp)
            {
                using (Graphics g = Graphics.FromImage(bmp))
                {
                    for (int ty = rect.Y; ty < rect.Y + rect.Height; ++ty)
                    {
                        for (int tx = rect.X; tx < rect.X + rect.Width; ++tx)
                        {
                            //DrawTile(g, 2);
                        }
                    }
                }
                this.ChunksTextures[x] = TextureCreator.FromBitmap(d._device, bmp);
            }

            return this.ChunksTextures[x];
        }

        private void DrawTilesChunk(DevicePanel d, int x, int y, int Transperncy)
        {
            Rectangle rect = GetTilesChunkArea(x, y);

            for (int ty = rect.Y; ty < rect.Y + rect.Height; ++ty)
            {
                for (int tx = rect.X; tx < rect.X + rect.Width; ++tx)
                {
                    Point p = new Point(tx, ty);
                    // We will draw those later
                    if (SelectedTiles.Contains(p) || TempSelectionTiles.Contains(p))
                    {
                        continue;
                    }
                    if (this._layer.Tiles[ty][tx] != 0xffff)
                    {
                        DrawTile(d, this._layer.Tiles[ty][tx], tx, ty, false, Transperncy);
                    }
                }
            }
        }

        private void DrawChunk(DevicePanel d, int x, int y, int Transperncy)
        {
            Rectangle rect = GetChunkArea(x, y);

            for (int ty = rect.Y; ty < rect.Y + rect.Height; ++ty)
            {
                for (int tx = rect.X; tx < rect.X + rect.Width; ++tx)
                {
                    Point p = new Point(tx, ty);
                    // We will draw those later
                    if (SelectedTiles.Contains(p) || TempSelectionTiles.Contains(p))
                    {
                        continue;
                    }
                    if (this._layer.Tiles[ty][tx] != 0xffff)
                    {
                        DrawTile(d, 1, tx, ty, false, Transperncy);
                    }
                }
            }
        }

        private void DrawSelectedTiles(DevicePanel d, int x, int y, int Transperncy)
        {
			foreach (Point p in SelectedTiles.GetChunkPoint(x, y))
			{
				if (d.IsObjectOnScreen(p.X * 16, p.Y * 16, 16, 16))
				{
					if (SelectedTilesValue.ContainsKey(p))
						DrawTile(d, SelectedTilesValue[p], p.X, p.Y, !TempSelectionDeselect || !TempSelectionTiles.Contains(p), Transperncy);


					else // It is still in the original place
						DrawTile(d, _layer.Tiles[p.Y][p.X], p.X, p.Y, !TempSelectionDeselect || !TempSelectionTiles.Contains(p), Transperncy);
				}
			}
            foreach (Point p in TempSelectionTiles.GetChunkPoint(x, y)) {
				if (d.IsObjectOnScreen(p.X * 16, p.Y * 16, 16, 16))
				{
					if (SelectedTiles.Contains(p))
					{
						continue;
					}
					DrawTile(d, _layer.Tiles[p.Y][p.X], p.X, p.Y, true, Transperncy);
				}
            }
        }

        public void Draw(DevicePanel d)
        {
			int Transperncy;


			if (EditorInstance.EditLayer != null && (EditorInstance.EditLayerA != this && EditorInstance.EditLayerB != this))
				Transperncy = 0x32;
			else if (EditorInstance.EditEntities.IsCheckedAll && EditorInstance.EditLayer == null && EditorInstance.UIModes.ApplyEditEntitiesTransparency)
				Transperncy = 0x32;
			else
				Transperncy = 0xFF;

			Rectangle screen = d.GetScreen();
			int startX = screen.X, startY = screen.Y;
			int width = screen.Width, height = screen.Height;

			if (screen.X < 0 || screen.Y < 0 || (screen.X + screen.Width > this.Width * 128) || Properties.Settings.Default.EntityFreeCam)
			{
				startX = 0;
				startY = 0;
				width = this.Width * 128;
				height = this.Height * 128;
			}

			int start_x = startX / (EditorConstants.TILES_CHUNK_SIZE * EditorConstants.TILE_SIZE);
			int end_x = Math.Min(DivideRoundUp(startX + width, EditorConstants.TILES_CHUNK_SIZE * EditorConstants.TILE_SIZE), TileChunksTextures[0].Length);
			int start_y = startY / (EditorConstants.TILES_CHUNK_SIZE * EditorConstants.TILE_SIZE);
			int end_y = Math.Min(DivideRoundUp(startY + height, EditorConstants.TILES_CHUNK_SIZE * EditorConstants.TILE_SIZE), TileChunksTextures.Length);

			for (int y = start_y; y < end_y; ++y)
			{
				for (int x = start_x; x < end_x; ++x)
				{
					if (d.IsObjectOnScreen(x * 256, y * 256, 256, 256))
					{
						Rectangle rect = GetTilesChunkArea(x, y);


					if (SelectedTiles.IsChunkUsed(x, y) || TempSelectionTiles.IsChunkUsed(x, y))
					{
						// TODO: If the full chunk isDrawTilesChunk selected, cache it
						// draw one by one
						DrawTilesChunk(d, x, y, Transperncy);
					}
					else
					{
						d.DrawBitmap(GetTilesChunkTexture(d, x, y), rect.X * EditorConstants.TILE_SIZE, rect.Y * EditorConstants.TILE_SIZE, rect.Width * EditorConstants.TILE_SIZE, rect.Height * EditorConstants.TILE_SIZE, false, Transperncy);
					}

						DrawSelectedTiles(d, x, y, Transperncy);
					}




				}
			}
            

            
            
        }

        /// <summary>
        /// Resizes both this EditorLayer, and the underlying SceneLayer
        /// </summary>
        /// <param name="width">The new width of the layer</param>
        /// <param name="height">The new height of the layer</param>
        public void Resize(ushort width, ushort height)
        {
            ushort oldWidth = Width;
            ushort oldHeight = Height;

            // first resize the underlying SceneLayer
            _layer.Resize(width, height);

            int oldWidthChunkSize = DivideRoundUp(oldWidth, EditorConstants.TILES_CHUNK_SIZE);
            int newWidthChunkSize = DivideRoundUp(Width, EditorConstants.TILES_CHUNK_SIZE);

            // now resize ourselves
            Array.Resize(ref TileChunksTextures, DivideRoundUp(Height, EditorConstants.TILES_CHUNK_SIZE));
            for (int i = DivideRoundUp(oldHeight, EditorConstants.TILES_CHUNK_SIZE); i < TileChunksTextures.Length; i++)
            {
                TileChunksTextures[i] = new Texture[oldWidthChunkSize];
            }

            for (int i = 0; i < TileChunksTextures.Length; i++)
            {
                Array.Resize(ref TileChunksTextures[i], newWidthChunkSize);
            }

            
            SelectedTiles = new PointsMap(Width, Height);
            TempSelectionTiles = new PointsMap(Width, Height);
        }

        public void Dispose()
        {
            foreach (Texture[] textures in TileChunksTextures)
                foreach (Texture texture in textures)
                    if (texture != null)
                        texture.Dispose();
            TileChunksTextures = null;
        }

        public void DisposeTextures()
        {
            foreach (Texture[] textures in TileChunksTextures)
            {
                for (int i = 0; i < textures.Length; ++i)
                {
                    if (textures[i] != null)
                    {
                        textures[i].Dispose();
                        textures[i] = null;
                    }
                }
            }
        }

        public void RefreshTileCount()
        {
            GlobalSelectedTiles = SelectedTiles.Count + TempSelectionTiles.Count;
            EditorInstance.EditorState.DeselectTilesCount = TempSelectionDeselectTiles.Count;
            EditorInstance.EditorState.SelectedTilesCount = GlobalSelectedTiles - EditorInstance.EditorState.DeselectTilesCount;
        }
    }
}
