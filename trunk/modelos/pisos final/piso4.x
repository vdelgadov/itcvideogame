xof 0303txt 0032

// DirectX - from MilkShape3D

Frame piso1Frame
{
    FrameTransformMatrix
    {
        1.000000,0.000000,0.000000,0.000000,
        0.000000,1.000000,0.000000,0.000000,
        0.000000,0.000000,1.000000,0.000000,
        0.000000,0.000000,0.000000,1.000000;;
    }

    Mesh piso1Mesh
    {
        4;
        5.000100;-4.999900;-0.000100;,
        5.000100;5.000100;-0.000100;,
        -4.999900;5.000100;-0.000100;,
        -4.999900;-4.999900;-0.000100;;
        2;
        3;0,2,1;,
        3;2,0,3;;

        MeshNormals
        {
            1;
            0.000000;0.000000;-1.000000;;
            2;
            3;0,0,0;,
            3;0,0,0;;
        }

        MeshTextureCoords
        {
            4;
            1.000000;1.000000;,
            1.000000;0.000000;,
            0.000000;0.000000;,
            0.000000;1.000000;;
        }

        MeshMaterialList
        {
            1;
            2;
            0,
            0;

            Material pisoR
            {
                0.584314;0.584314;0.584314;1.000000;;
                12.800000;
                0.898039;0.898039;0.898039;;
                0.000000;0.000000;0.000000;;

                TextureFileName
                {
                    "pisoT1_04_DIFFUSE.bmp";
                }
            }
        }
    }
}
