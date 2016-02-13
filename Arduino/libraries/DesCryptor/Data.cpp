#include "Data.h"

Data::Data()
{

}

Data::Data(byte *data, byte size)
{

}

byte Data::operator [](byte index)
{
    return data_[index];
}
