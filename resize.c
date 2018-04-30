// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    float f = atof(argv[1]);

    if (f <= 0 || f > 100)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        fprintf(stderr, "f < 0 and n > 100\n");
        return 2;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }

    BITMAPFILEHEADER out_bf = bf;
    out_bf.bfOffBits = 54;

    BITMAPINFOHEADER out_bi = bi;

    if (f >= 1.0)
    {
        out_bi.biWidth = bi.biWidth * (int)(f);
        out_bi.biHeight = bi.biHeight * (int)(f);
    }
    else
    {
        out_bi.biWidth = round(bi.biWidth * f);
        out_bi.biHeight = round(bi.biHeight * f);
    }


    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int out_padding = (4 - (out_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    out_bi.biSizeImage = ((out_bi.biWidth * sizeof(RGBTRIPLE)) + out_padding) * abs(out_bi.biHeight);

    out_bf.bfSize = out_bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&out_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADE
    fwrite(&out_bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    //int row = 1;
    //int col = 1;
    int row = 0;
    int col = 0;
    // iterate over infile's scanlines
    // Adding new size before the while loop, subtracting old size in the loop.
    for (int i = 0; i < abs(bi.biHeight); i++)
    {
        row += abs(out_bi.biHeight);

        //printf("i: %i\n", i);
        //printf("row: %i\n", row);
        while (row > 0)
        {
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                col += out_bi.biWidth;
                //printf("col: %i\n", col);
                while (col > 0)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    //col--;
                    col-=bi.biWidth;
                }
            }
            // write padding to outfile
            for (int k = 0; k < out_padding; k++)
            {
                fputc(0x00, outptr);
            }
            row-=abs(bi.biHeight);
            fseek(inptr, -(bi.biWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
        }
        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // bring back cursor for new line
        fseek(inptr, (bi.biWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
        printf("\n");
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
