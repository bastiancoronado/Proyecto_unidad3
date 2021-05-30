using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Sytm : MonoBehaviour
{
    ClkComm clk;
    BmeComm bme;

    Renderer colors;
    Transform tr;
    float h = 0, s = 0, v = 0;
    float[] modes;

    public Text date;
    public Text tem;
    public Text hum;

    public ParticleSystem red;
    public ParticleSystem blue;

    float t = 0;
    float last = 0;

    void Start()
    {
        clk = GameObject.Find("comm").GetComponent<ClkComm>();
        bme = GameObject.Find("comm").GetComponent<BmeComm>();
        colors = GetComponentInChildren<Renderer>();
        tr = GameObject.Find("bola").GetComponent<Transform>();
    }

    void Update()
    {
        if (last + 1 < t)
        {

            date.text = clk.Date;
            modes = timer(date.text);


            v = Mathf.Lerp(0.5f, 1, modes[0] == 12 ? 0 : modes[0] / 11);
            s = Mathf.Lerp(0.5f, 1, modes[1] / 59);
            h = modes[2] / 59;


            tem.text = bme.Tempeture.ToString();
            hum.text = bme.Humedity.ToString();

            colors.material.color = Color.HSVToRGB(h, s, v);//(0.5f, 0.5f, 0.5f);
            //Debug.Log("h: " + h + " s: " + s + " v: " + v);

            tr.localScale = Vector3.one * Mathf.Lerp(10, 1, modes[1] / 59); ;

            if (!red.isPlaying && bme.Tempeture > 26f)red.Play();
            if (!blue.isPlaying && bme.Humedity > 65f)blue.Play();
            last = t;
        }
        t += Time.deltaTime;
    }

    static float[] timer(string c)
    {
        float[] tmr = {0, 0, 0, 0};
        string[] a = c.Split(':');
        string[] b = a[2].Split(' ');

        tmr[0] = int.Parse(a[0]);
        tmr[1] = int.Parse(a[1]);
        tmr[2] = int.Parse(b[0]);
        tmr[3] = b[1] == "Pm" ? 1 : 0;

        return tmr;
    }
}
