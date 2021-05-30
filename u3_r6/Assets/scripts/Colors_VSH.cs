using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Colors_VSH : MonoBehaviour
{
    Renderer colors;
    public float h = 0;
    public float s = 0;
    public float v = 0;

    void Start()
    {
        colors = GetComponent<Renderer>();
    }


    void Update()
    {
        colors.material.color = Color.HSVToRGB(h, s, v);
    }
}
