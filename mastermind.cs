using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace Mastermind
{
    public partial class Form1 : Form
    {
        public int count = 0;
        public static int Turns = 36;   //  360/angle
        public static double TurnAngle = 10;
        public static double DistanceForward = 25;   //cm
        public double angle = 0;
        int[] radius1 = new int[Turns];
        int[] radius2 = new int[Turns];
        double[] theta1 = new double[Turns];
        double[] theta2 = new double[Turns];
        //double[] positionX = new double[Turns];    //array for Cartesian X position
        //double[] positionY = new double[Turns];    //array for Cartesian Y position
        //double[] position2X = new double[Turns];    //array for Cartesian X position
        //double[] position2Y = new double[Turns];    //array for Cartesian Y position
        //double[] differenceX = new double[Turns];
        //double[] differenceY = new double[Turns];
        double[] differenceRadius = new double[Turns];

        public Form1()
        {
            InitializeComponent();
            serialPort1.PortName = "COM7";
            serialPort1.BaudRate = 9600;
            serialPort1.Parity = Parity.None;
            serialPort1.Handshake = Handshake.None;
            //serialPort1.ErrorReceived += SerialPort1_ErrorReceived;
            try
            {
                if (!serialPort1.IsOpen)
                    serialPort1.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            serialPort1.WriteLine("R");
            serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(DataRecieved);
        }

        /// <summary>
        /// Handles the data recieved and starts the mapping program
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DataRecieved(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            SerialPort spl = (SerialPort)sender;
            //Console.Write(spl.ReadLine());
            string input = "x";
            while (input != "FIRSTSPIN")
            {
                input = spl.ReadLine();
            }
            /*First Spin*/
            for (int j = 0; j < Turns; j++)
            {
                if (spl.ReceivedBytesThreshold > 0)
                {
                    int val = int.Parse(spl.ReadLine());
                    for (int i = 0; i < Turns; i++)
                    {
                        radius1[i] = val;
                        theta1[i] = angle * (Math.PI / 180);     //turn degree to radians
                        Console.WriteLine(val);
                    }
                    angle += TurnAngle;
                }
            }
            while (input != "SECONDSPIN")
            {
                input = spl.ReadLine();
            }
            /*Second Spin*/
            for (int k = 0; k < Turns; k++)
            {
                if (spl.ReceivedBytesThreshold > 0)
                {
                    int val = int.Parse(spl.ReadLine());
                    for (int i = 0; i < Turns; i++)
                    {
                        radius2[i] = val;
                        theta2[i] = angle * (Math.PI / 180);     //turn degree to radians
                        Console.WriteLine(val);
                    }
                    angle += TurnAngle;
                }
            }
            //SecondPosition(radius2, theta2);
            ArrayDifference();
        }


        private void DriveForward()
        {
            serialPort1.Write("1");
        }
        private void ScanAgain()
        {
            serialPort1.Write("2");
        }
        private void Launch()
        {
            serialPort1.Write("0");
        }

        /// <summary>
        /// Finds the difference between two x arrays and two y arrays. The difference will show us where object searching for is.
        /// </summary>
        void ArrayDifference()
        {
            for (int i = 0; i < Turns; i++)
            {
                differenceRadius[i] = radius1[i] - radius2[i];
            }
            Search();
        }

        /// <summary>
        /// Searches for the object and intializes Destroy()
        /// </summary>
        void Search()
        {
            int TargetRadius = 0;
            for (int i = 0; i < differenceRadius.Length; i++)
            {
                if (Math.Abs(differenceRadius[i]) > Math.Abs(differenceRadius[i - 1]))    //If previous difference is greater than chances are higher that it is object
                {
                    TargetRadius = i;    //gives index of differenceRadius that has greatest difference i.e. most likely the object searching for
                }
            }
            Console.WriteLine(TargetRadius);
            Destroy(TargetRadius);
        }

        /// <summary>
        /// The final turn towards object, drive towards it and shoot.
        /// </summary>
        /// <param name="r"></param>
        void Destroy(int r)
        {
            int turnNumber = r;
            string turnString;
            double TargetPos = radius2[turnNumber];    //Our actual distance from object
            if (turnNumber < 10)
            {
                turnString = "0" + turnNumber.ToString();
            }
            else
            {
                turnString = turnNumber.ToString();
            }
            serialPort1.WriteLine("2" + turnString);            //Send turn number for robot to turn to object location
            Console.WriteLine("2" + turnString);
        }


    }
}
