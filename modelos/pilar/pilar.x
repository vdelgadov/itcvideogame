xof 0303txt 0032

// DirectX - from MilkShape3D

Frame pilarFrame
{
    FrameTransformMatrix
    {
        1.000000,0.000000,0.000000,0.000000,
        0.000000,1.000000,0.000000,0.000000,
        0.000000,0.000000,1.000000,0.000000,
        0.000000,0.000000,0.000000,1.000000;;
    }

    Mesh pilarMesh
    {
        11;
        -1.337332;-1.069845;-7.874116;,
        -1.337332;-1.069845;-0.000100;,
        1.337532;-1.069845;-0.000100;,
        1.337532;-1.069845;-7.874116;,
        1.337532;1.070045;-0.000100;,
        1.337532;1.070045;-7.874116;,
        1.337532;1.070045;-7.874116;,
        1.337532;1.070045;-0.000100;,
        -1.337332;1.070045;-0.000100;,
        -1.337332;1.070045;-7.874116;,
        1.337532;1.070045;-7.874116;;
        8;
        3;0,2,1;,
        3;2,0,3;,
        3;3,4,2;,
        3;4,3,5;,
        3;6,8,7;,
        3;8,10,9;,
        3;9,1,8;,
        3;1,9,0;;

        MeshNormals
        {
            4;
            0.000000;-1.000000;0.000000;,
            1.000000;0.000000;0.000000;,
            0.000000;1.000000;0.000000;,
            -1.000000;0.000000;0.000000;;
            8;
            3;0,0,0;,
            3;0,0,0;,
            3;1,1,1;,
            3;1,1,1;,
            3;2,2,2;,
            3;2,2,2;,
            3;3,3,3;,
            3;3,3,3;;
        }

        MeshTextureCoords
        {
            11;
            0.499075;0.093065;,
            0.499075;0.899603;,
            0.745231;0.899606;,
            0.745231;0.093069;,
            0.991344;0.899606;,
            0.991344;0.093069;,
            0.006775;0.093109;,
            0.006775;0.899647;,
            0.252958;0.899602;,
            0.252958;0.093064;,
            0.006775;0.093109;;
        }

        MeshMaterialList
        {
            1;
            8;
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0;

            Material pilar
            {
                0.584314;0.584314;0.584314;1.000000;;
                12.800000;
                0.898039;0.898039;0.898039;;
                0.000000;0.000000;0.000000;;

                TextureFileName
                {
                    "pilarDiffuse.bmp";
                }
            }
        }
    }
}
