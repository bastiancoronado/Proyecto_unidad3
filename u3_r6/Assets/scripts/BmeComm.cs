using System.Collections;

using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;
using System;
using UnityEngine.UI;

public class BmeComm : MonoBehaviour
{
    public Text ipBme;
    public Text portBme;

    private static BmeComm instance;
    private Thread receiveThread;
    private UdpClient receiveClient;
    private IPEndPoint receiveEndPoint;
    string ip = "";// = "192.168.0.5";
    int receivePort = 0;// = 5051;
    private bool isInitialized;
    private Queue receiveQueue_T;
    private Queue receiveQueue_H;

    float tempeture;
    float humedity;

    public float Tempeture { get => tempeture; set => tempeture = value; }
    public float Humedity { get => humedity; set => humedity = value; }


    private void Initialize()
    {
        instance = this;
        receiveEndPoint = new IPEndPoint(IPAddress.Parse(ip), receivePort);
        receiveClient = new UdpClient(receivePort);
        receiveQueue_T = Queue.Synchronized(new Queue());
        receiveQueue_H = Queue.Synchronized(new Queue());
        receiveThread = new Thread(new ThreadStart(ReceiveDataListener));
        receiveThread.IsBackground = true;
        receiveThread.Start();
        isInitialized = true;
    }

    private void ReceiveDataListener()
    {
        while (true)
        {
            try
            {
                byte[] t = new byte[4];
                byte[] h = new byte[4];
                float temp, humd;
                byte[] data = receiveClient.Receive(ref receiveEndPoint);
                if (data.Length >= 8)
                {
                    for (int i = 0; i < h.Length; i++) h[i] = data[h.Length + i];
                    for (int i = 0; i < t.Length; i++) t[i] = data[i];

                    if (!BitConverter.IsLittleEndian) 
                    { 
                        Array.Reverse(t);
                        Array.Reverse(h);
                    }

                    temp = System.BitConverter.ToSingle(t, 0);
                    humd = System.BitConverter.ToSingle(h, 0);

                    receiveQueue_H.Enqueue(humd);
                    receiveQueue_T.Enqueue(temp);

                }                           



            }
            catch (System.Exception ex)
            {
                Debug.Log(ex.ToString());
            }
        }
    }


    private void OnDestroy()
    {
        TryKillThread();
    }

    private void OnApplicationQuit()
    {
        TryKillThread();
    }

    private void TryKillThread()
    {
        if (isInitialized)
        {
            receiveThread.Abort();
            receiveThread = null;
            receiveClient.Close();
            receiveClient = null;
            Debug.Log("Thread killed");
            isInitialized = false;
        }
    }

    void Configurate()
    {
        ip = ipBme.text;
        receivePort = int.Parse(portBme.text);
        //Debug.Log(ip + " " + receivePort);
    }

    void Start()
    {
        Configurate();
        Initialize();
    }


    void Update()
    {

        if (isInitialized)
        {
            if (receiveQueue_H.Count != 0 && receiveQueue_T.Count != 0)
            {
                humedity = (float)receiveQueue_H.Dequeue();
                tempeture = (float)receiveQueue_T.Dequeue();
            }
        }
    }


}
