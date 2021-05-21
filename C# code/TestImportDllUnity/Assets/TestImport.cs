using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;


public class TestImport : MonoBehaviour
{
    [DllImport("dll", EntryPoint = "addInC")]
    static extern float addInC(float a, float b);

    [DllImport("dll", EntryPoint = "instance_create")]
    static extern IntPtr instance_create();

    [DllImport("dll", EntryPoint = "add")]
    static extern float add(IntPtr tl, float a, float b);

    [DllImport("dll", EntryPoint = "substract")]
    static extern float substract(IntPtr tl, float a, float b);
    // Start is called before the first frame update
    void Start()
    {
        float a = addInC(5.5f, 3.3f);

        IntPtr tl = instance_create();
        float b = add(tl, 5.5f, 3.3f);
        float c = substract(tl, 5.5f, 3.3f);

        Debug.Log("Hey, this is result from c++ :" + a + " " + b + " " + c);
    }

    // Update is called once per frame
    void Update()
    {

    }
}
