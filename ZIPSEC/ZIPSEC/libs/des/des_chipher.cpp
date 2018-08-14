#include "des_chipher.h"

DES_Cipher::DES_Cipher(std::string _key, int _mode) {
    mode = _mode;
    if (mode > 1) {
        println("zipsec-error: crypting mode is invalid, options are des and 3des");
        exit(0);
    }
    int length = (int) _key.length();
    if (mode == MODE_DES && length != 8) {
        println("zipsec-error: la chiave per l'algoritmo DES deve essere lunga 8 caratteri, quella inserita ha lunghezza pari a "
                        << length << " caratteri");
        exit(0);
    } else if (length != 8 && length != 16 && length != 24) {
        println("la chiave per l'algoritmo 3des deve essere lunga 8 o 16 o 24 caratteri, quella inserita ha lunghezza pari a "
                        << length << " caratteri");
        exit(0);
    }
    key = _key;

}

std::string DES_Cipher::crypt(const std::string &original) const {
    return exec_crypt(original, true);
}

std::string DES_Cipher::exec_crypt(const std::string &original, bool crypt) const {

    //SIZE ADJUSTMENTS

    auto length = (size_t) original.length();
    size_t missing_length = (length % 8 != 0) ? 8 - (length % 8) : 0;
    size_t valid_length = length + missing_length;

    char *valid_length_array = new char[valid_length];
    std::string modified_array;

    const char *orig = original.c_str();
    for (size_t index = 0; index < length; index++) {
        valid_length_array[index] = orig[index];
    }

    //END
    size_t perc = 0;
    size_t prevperc = 0;
    if (mode == MODE_DES || key.length() == 8) {
        ulong k = 0;
        ulong_from_str(key.c_str(), k)
        Encoder encoder = Encoder(k);
        for (size_t crypt_index = 0; crypt_index < length; crypt_index += 8) {

            //perc printing
            if (length < 100) perc = 100;
            else perc = crypt_index / (length / 100);
            if (perc % 10 == 0 && perc != prevperc) {
                if (perc != 0)
                    rollback_line();
                print(perc << " % ... ");
                prevperc = perc;
            }
            //crypting
            ulong to_crypt = 0;

            for (uchar a = 0; a < 8; a++)
                to_crypt |= (((ulong) valid_length_array[crypt_index + a] & 0xFF) << ((7 - a) * 8));
            ulong crypted =encoder.encode(to_crypt, !crypt);
            for (uchar a = 0; a < 8; a++)
                modified_array += (char) get_char(crypted, a);
        }
    } else {

        size_t offset1 = 0;
        size_t offset2 = 0;
        size_t offset3 = 0;
        if (key.length() >= 16)
            offset2 = 8;
        if (key.length() == 24)
            offset3 = 16;

        //primo passaggio con la prima chiave
        Encoder encoder = Encoder(get_ulong_from_string(key.c_str(), (crypt) ? offset1 : offset3));
        Encoder encoder2 = Encoder(get_ulong_from_string(key.c_str(), offset2));
        Encoder encoder3 = Encoder(get_ulong_from_string(key.c_str(), (crypt) ? offset3 : offset1));

        for (size_t crypt_index = 0; crypt_index < valid_length; crypt_index += 8) {

            //perc printing
            if (valid_length < 100) perc = 100;
            else perc = crypt_index / (valid_length / 100);
            if (perc % 10 == 0 && perc != prevperc) {
                if (perc != 0)
                    rollback_line();
                print(perc << " % ... ");
                prevperc = perc;
            }

            ulong to_crypt = 0;
            for (uchar a = 0; a < 8; a++)
                to_crypt |= get_ulong_from_char((ulong) valid_length_array[crypt_index + a], a);
            ulong crypted = encoder.encode(to_crypt,!crypt);
            crypted = encoder2.encode(crypted,crypt);
            crypted = encoder3.encode(crypted, !crypt);

            for (uchar a = 0; a < 8; a++)
                modified_array += (char) get_char(crypted, a);
        }

    }

    println("");
    delete[] valid_length_array;
    return modified_array;
}

std::string DES_Cipher::decrypt(const std::string &original) const {
    return exec_crypt(original, false);
}

ulong DES_Cipher::get_ulong_from_string(const char *const original, size_t offset) {
    ulong string_as_ulong = 0;
    for (ushort a = 0; a < 8; a++)
        string_as_ulong |= get_ulong_from_char((ulong) original[offset + a], a);
    return string_as_ulong;
}

std::string DES_Cipher::get_string_from_ulong(ulong original) {
    std::string ulong_as_string;
    for (ushort a = 0; a < 8; a++)
        ulong_as_string += (char) get_char(original, a);
    return ulong_as_string;
}

void DES_Cipher::crypt_string(const char *str,const size_t length, const bool crypt, std::string key, std::string* toReturn){
    //ACTUAL CRYPTING
    ulong k = 0;
    ulong_from_str(key.c_str(), k)
    Encoder encoder = Encoder(k);

    //= (crypt)? encoder.encode : encoder.decode;

    for (size_t crypt_index = 0; crypt_index < length; crypt_index += 8) {

        /*//perc printing
        if (length < 100) perc = 100;
        else perc = crypt_index / (length / 100);
        if (perc % 10 == 0 && perc != prevperc) {
            if (perc != 0)
                rollback_line();
            print(perc << " % ... ");
            prevperc = perc;
        }*/
        //crypting
        ulong to_crypt = 0;

        for (uchar a = 0; a < 8; a++)
            to_crypt |= (((ulong) str[crypt_index + a] & 0xFF) << ((7 - a) * 8));

        ulong crypted = encoder.encode(to_crypt, !crypt);
        for (uchar a = 0; a < 8; a++)
            (*toReturn) += (char) get_char(crypted, a);
    }
    //END
}
