﻿// MIT License
// 
// Copyright(c) 2017 Luciano (Xeeynamo) Ciccariello
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Part of this software belongs to XeEngine toolset and United Lines Studio
// and it is currently used to create commercial games by Luciano Ciccariello.
// Please do not redistribuite this code under your own name, stole it or use
// it artfully, but instead support it and its author. Thank you.

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Timers;

namespace AnimationEditor.Services
{
    public class PlaybackService
    {
        private MainWindow ParentInstance;
        private string _animationName;
        public int _frameIndex;
        private Animation.AnimationEntry _currentAnimation;
        private Animation.Frame _currentFrame;
        private Dictionary<string, Animation.AnimationEntry> _dicAnimations;
        private long _previousTime;
        private long _discardedTime;
        private object _lockTime = new object();

        public event Action<PlaybackService> OnFrameChanged;

        public Animation AnimationData { get; set; }

        public Timer Timer { get; private set; }
        public Stopwatch Stopwatch { get; private set; }

        /// <summary>
        /// Get or set if the animation system is running
        /// </summary>
        public bool IsRunning
        {
            get => Timer.Enabled;
            set
            {
                if (Timer.Enabled != value)
                {
                    if (value)
                    {
                        Stopwatch.Start();
                    }
                    else
                    {
                        Stopwatch.Stop();
                    }
                    Timer.Enabled = value;
                }
            }
        }

        /// <summary>
        /// Get the number of frames per second of selected animation
        /// </summary>
        public int FramesPerSecond
        {
            get
            {
                var speed = CurrentAnimation?.SpeedMultiplyer ?? 0;
                return speed > 0 ? speed / 4 : 0;
            }
        }

        /// <summary>
        /// Get or set the name of current animation
        /// </summary>
        public string Animation
        {
            get => _animationName;
            set
            {
                _animationName = value;
                // Validate the name
                if (!string.IsNullOrWhiteSpace(value))
                {
                    // Get the animation object from its name
                    if (_dicAnimations.TryGetValue(value, out _currentAnimation))
                    {
                        // Reset the timer
                        if (IsRunning)
                        {
                            Stopwatch.Restart();
                        }
                        else
                        {
                            Stopwatch.Reset();
                        }
                        // Reset the frame index
                        _frameIndex = -1;
                        FrameIndex = 0;
                    }
                }
                else
                {
                    Stopwatch.Reset();
                }
            }
        }

        /// <summary>
        /// Get or set the current frame index
        /// </summary>
        public int FrameIndex
        {
            get => _frameIndex;
            set
            {
                if (_frameIndex != value)
                {
                    _frameIndex = value;
                    _currentFrame = CurrentAnimation?.Frames
                        .Skip(_frameIndex).FirstOrDefault();
                    OnFrameChanged?.Invoke(this);
                }
            }
        }

        /// <summary>
        /// Get the current animation object loaded
        /// </summary>
        public Animation.AnimationEntry CurrentAnimation => _currentAnimation;

        public Animation.Frame CurrentFrame => _currentFrame;

        /// <summary>
        /// Initialize a new instance of an animation service
        /// </summary>
        /// <param name="animData">Animation data where information are loaded</param>
        public PlaybackService(Animation animData, MainWindow instance)
        {
            ParentInstance = instance;
            AnimationData = animData;
            Timer = new Timer(1);
            Timer.Elapsed += Timer_Elapsed;
            Stopwatch = new Stopwatch();

            _dicAnimations = AnimationData.Animations
                .ToDictionary(x => x.AnimName, x => x);

            Timer.Enabled = true;
        }

        /// <summary>
        /// Change the frame's reference from the specified index
        /// </summary>
        /// <param name="index"></param>
        /// <param name="frame"></param>
        public void SetFrame(int index, string frameName)
        {
            if (FrameIndex != index && index >= 0)
                FrameIndex = index;
        }

        private void Timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            if (IsRunning)
            {
                var currentTime = Stopwatch.ElapsedMilliseconds;
                lock (_lockTime)
                {
                    var elapsed = _discardedTime + (currentTime - _previousTime);
                    _previousTime = currentTime;
                    _discardedTime = Update(elapsed);
                }
            }
        }

        /// <summary>
        /// Update animation state by the specified number of milliseconds.
        /// </summary>
        /// <param name="ms">Number of elapsed milliseconds.</param>
        /// <returns>Discarded milliseconds.</returns>
        private long Update(long ms)
        {
            var curAnim = CurrentAnimation;
            if (ms < 0 || curAnim == null ||
                (CurrentAnimation?.SpeedMultiplyer ?? 0) <= 0 && !MainWindow.isForcePlaybackOn)
                return 0;

            int framesCount = curAnim.Frames.Count();
            int loop = curAnim.LoopIndex;
            if (framesCount <= 0)
                return 0;

            const int Divisor = 1024;
            var baseSpeed = (MainWindow.isForcePlaybackOn ? MainWindow.ForcePlaybackSpeed : CurrentAnimation.SpeedMultiplyer);
            long prevMs;
            do
            {
                prevMs = ms;
                var frameSpeed = (MainWindow.isForcePlaybackOn ? MainWindow.ForcePlaybackDuration : CurrentFrame?.Delay ?? 256);

                if (frameSpeed > 0)
                {
                    var realSpeed = (baseSpeed * 64 / frameSpeed);
                    if (realSpeed > 0)
                    {
                        var acutalSpeed = Divisor / realSpeed;
                        if (ms > acutalSpeed && acutalSpeed > 0)
                        {
                            ms -= acutalSpeed;
                            FrameIndex = BoundFrameIndex(FrameIndex + 1, curAnim.LoopIndex  , framesCount);
                            OnFrameChanged?.Invoke(this);
                        }
                    }
                }
            } while (prevMs != ms);
            return ms;
        }

        /// <summary>
        /// Move between frames index without to exit from frames boundaries.
        /// </summary>
        /// <param name="index">Frame index to adjust.</param>
        /// <param name="loop">Loop index</param>
        /// <param name="framesCount">Total number of frames.</param>
        /// <returns>New frame index</returns>
        private int BoundFrameIndex(int index, int loop, int framesCount)
        {
            if (index >= framesCount)
            {
                index = loop >= index ? framesCount - 1 : loop;
            }
            else if (index < 0)
            {
                index = 0;
            }
            return index;
        }
    }
}
