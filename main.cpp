#include <iostream>
#include <vector>

int main()
{
    std::string SecretKey = "lockedoutfromreadingcode";
    char SecretKeyGrid[10][10];
    std::string Message = "Hello all";
    std::vector<int16_t> Encode{};

    // Set up Grid Values
    for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < 10; col++)
            SecretKeyGrid[row][col] = (int16_t)(32 + (col + (row * 10)));
    }

    // Swap around the values based on our secret key, so that all the secret keys are at the start
    int uniqueIndex = 0;
    for (int keyIndex = 0; keyIndex < SecretKey.length(); keyIndex++)
    {
        for (int row = 0; row < 10; row++)
        {
            for (int col = 0; col < 10; col++)
            {
                if (SecretKeyGrid[row][col] == SecretKey[keyIndex])
                {
                    int16_t backup = SecretKeyGrid[uniqueIndex / 10][uniqueIndex % 10];
                    SecretKeyGrid[uniqueIndex / 10][uniqueIndex % 10] = SecretKey[keyIndex];
                    SecretKeyGrid[row][col] = backup;
                    uniqueIndex++;
                    row = 10;
                    col = 10;
                }
            }
        }
    }




    // Encode to Unsigned Char Vector
    for (int msgIndex = 0; msgIndex < Message.length(); msgIndex++){
        char input = Message[msgIndex];
        for (int row = 0; row < 10; row++)
        {
            for (int col = 0; col < 10; col++)
            {
                if (SecretKeyGrid[row][col] == (int16_t)input)
                {
                    if (msgIndex == 0)
                    {
                        Encode.push_back((row * 10) + col);
                    }
                    else
                    {
                        int16_t val = Encode.back();
                        int16_t value = (((row * 10) + col) - (int)val);
                        if (value<=0)
                        {
                            value += 256;
                        }
                        Encode.push_back(value);
                    }
                    row = 10;
                    col = 10;
                }
            }
        }
    }

    // At this point we should have a vector of int16_t (2 byte values)
    // It would be great to be able to story this data somehow, for example in binary
    // storing it as characters wont be possible as some of the characters dont have output values
    // so in that case we may need to read them in as int values (4 bytes) or store in binary.


    // Decode to string
    std::string Decoded;

    for (int EncodeIndex = 0; EncodeIndex < Encode.size();EncodeIndex++)
    {
        int gridIndex = Encode[EncodeIndex];
        if (EncodeIndex==0)
        {
            int row = gridIndex / 10;
            int col = gridIndex % 10;
            Decoded += SecretKeyGrid[row][col];
        }
        else
        {
            int gridIndex2 = Encode[EncodeIndex - 1] + gridIndex;
            if (gridIndex2>=256){
                gridIndex2 -= 256;
            }
            int row = gridIndex2 / 10;
            int col = gridIndex2 % 10;
            Decoded += SecretKeyGrid[row][col];
        }
    }
}