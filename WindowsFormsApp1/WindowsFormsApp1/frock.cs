using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Rebar;

namespace WindowsFormsApp1
{
    
    public enum ZeroSetting
    {
        Zero = 0,
        Ninty = 90,
        One80 = 180,
        Two70 = 270,
    }
    public enum Direction
    {
        Sclockwise = 1,
        Counterclockwise = -1
    }
    internal class frock : Control
    {
        private int _AngleSet = 0;//零点设定
        private int _tickCount = 60;//
        private Direction _direction = Direction.Sclockwise;
        public ZeroSetting _zeroSet = ZeroSetting.Zero;


        [Browsable(true)]
        [DefaultValue(0)]
        [Description("零点设定")]
        public int AngelSet
        {
            get
            {
                return _AngleSet;
            }
            set
            {
                _AngleSet = value;
                Invalidate();
            }
        }





        [DefaultValue(ZeroSetting.Zero)]
        public ZeroSetting ZeroSet
        {
            get { return _zeroSet; }
            set 
            { 
                _zeroSet = value; 
                Invalidate();
            }
        }


        /// <顺逆方向控制>
        /// 
        /// </顺逆方向控制>
        public Direction Directionz
        {
            get { return _direction; }
            set 
            {
                _direction = value;
                Invalidate();
            }
        }
        /// <刻度线数量>
        /// 外部类访问内部变量方法
        /// </刻度线数量>
        /// <param name="TickCount">外部访问此类中_tickCount变量方法</param>
        [Browsable(true)]
        [DefaultValue(60)]
        [Description("刻度线数量")]
        public int TickCount 
        { 
            get { return _tickCount; } 
            set 
            {
                if (value <= 0) return;
                _tickCount = value;
                Invalidate();
            } 
        }

        /// <frock>
        /// 构造方法
        /// </frock>
        public frock()
        {
            this.DoubleBuffered = true;//防闪
            this.Size = new Size(120, 120);
            //this.Resize += (s, e) => Invalidate();//改变大小就重绘
        }

        /// <OnPaint>
        /// 
        /// </OnPaint>
        /// <param name="e"></param>
        protected override void OnPaint(PaintEventArgs e)
        {
            const int tickLen = 12;//⚪内刻度长度
            const int tickLenLarge = 20;//⚪内刻度长度大
            const int Text_offset = 5;//文本⚪外边距

            base.OnPaint(e);
            var g  = e.Graphics;
            g.SmoothingMode = SmoothingMode.AntiAlias;

            float cx = this.ClientSize.Width/2;
            float cy = this.ClientSize.Height/2;
            float r = Math.Min(cx, cy) - 20;

            g.DrawEllipse(Pens.Black, cx - r, cy - r, r * 2, r * 2);

            for (int i = 0; i < _tickCount; i++)
            {
                double angleDeg = i ;
                DrawTick(g, cx, cy, r, angleDeg, tickLen);
            }
            for (int i = 0; i < 72; i++)
            {
                double angleDeg = i*5;
                DrawTick(g, cx, cy, r, angleDeg, tickLenLarge);
            }
            var labels = new[]
            {
                ("0",0),
                ("90",90),
                ("180",180),
                ("270",270)
            };
            var sf = new StringFormat { Alignment = StringAlignment.Center, LineAlignment = StringAlignment.Center };
            var brush = new SolidBrush(Color.Black);
            foreach (var (txt, deg) in labels)
            {
                double rad = (int)_direction * (deg + (int)_zeroSet) * Math.PI / 180;
                float x = cx + (float)((r + (float)Text_offset) * Math.Cos(rad));
                float y = cy + (float)((r + (float)Text_offset) * Math.Sin(rad));
                e.Graphics.DrawString(txt, this.Font, brush, x, y, sf);
            }
        }


        /// <画线方法>
        /// 
        /// </画线方法>
        /// <param name="g"></param>
        /// <param name="cx"></param>
        /// <param name="cy"></param>
        /// <param name="radius"></param>
        /// <param name="angleDeg"></param>
        /// <param name="tickLen"></param>
        private void DrawTick(Graphics g, float cx, float cy, float radius, double angleDeg, int tickLen)
        {
            double rad = (angleDeg) * Math.PI / 180;
            float x1 = cx + (float)(radius * Math.Cos(rad));
            float y1 = cy + (float)(radius * Math.Sin(rad));

            float x2 = cx + (float)((radius - tickLen) * Math.Cos(rad));
            float y2 = cy + (float)((radius - tickLen) * Math.Sin(rad));

            g.DrawLine(Pens.Black, x1, y1, x2, y2);
        }


    }
}
