using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Config : MonoBehaviour
{
    public GameObject communication;
    public GameObject canvas;
    public GameObject system;

    public void commStart()
    {
        communication.SetActive(true);
        canvas.SetActive(false);
        system.SetActive(true);
    }


    void Start()
    {
        
    }


    void Update()
    {
        
    }
}
