﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace ManiacEditor
{
	class ImageExtensions
	{


		public static Image ScaleImage(Image image, int maxWidth, int maxHeight)
		{
			var ratioX = (double)maxWidth / image.Width;
			var ratioY = (double)maxHeight / image.Height;
			var ratio = Math.Min(ratioX, ratioY);

			var newWidth = (int)(image.Width * ratio);
			var newHeight = (int)(image.Height * ratio);

			var newImage = new Bitmap(maxWidth, maxHeight);
			using (var graphics = Graphics.FromImage(newImage))
			{
				// Calculate x and y which center the image
				int y = (maxHeight / 2) - newHeight / 2;
				int x = (maxWidth / 2) - newWidth / 2;

				// Draw image on x and y with newWidth and newHeight
				graphics.DrawImage(image, x, y, newWidth, newHeight);
			}

			return newImage;
		}


	}
}
