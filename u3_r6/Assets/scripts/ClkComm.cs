using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;
using UnityEngine.UI;

public class ClkComm : MonoBehaviour
{
    public Text ipClk;
    public Text porClk;

    private static ClkComm instance;
    private Thread receiveThread;
    private UdpClient receiveClient;
    private IPEndPoint receiveEndPoint;
    string ip = "";// = "192.168.0.5";
    int receivePort = 0;// = 5052;
    private bool isInitialized;
    private Queue receiveQueue;

    string date;

    public string Date { get => date; set => date = value; }

    private void Initialize()
    {
        instance = this;
        receiveEndPoint = new IPEndPoint(IPAddress.Parse(ip), receivePort);
        receiveClient = new UdpClient(receivePort);
        receiveQueue = Queue.Synchronized(new Queue());
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
                byte[] data = receiveClient.Receive(ref receiveEndPoint);
                if (data.Length >= 8)
                {

                    string tm = btTotime(data);
                    receiveQueue.Enqueue(tm);
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
        ip = ipClk.text;
        receivePort = int.Parse(porClk.text);
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
            if (receiveQueue.Count != 0) { 
                date = (string)receiveQueue.Dequeue();
                //Debug.Log(date); 
            } 
        }
        
    }
    
    static string btTotime(byte[] tm)
    {
        string[] mounth = { "-", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
        string[] day = {"-", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado", "Domingo" };
        string clk = "";
        for (short i = 0; i < 8; i++) clk += tm[i].ToString() + ",";
        string[] rk = clk.Split(',');
        clk = rk[2] + ":" + rk[1] + ":" + rk[0] + (rk[3] == "1"? " Pm " : " Am ") + day[int.Parse(rk[4])] + " " + rk[5] + "/" + mounth[int.Parse(rk[6])] + "/20" + rk[7];
        return clk;
    }

}
