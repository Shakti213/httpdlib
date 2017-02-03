#include <iostream>

bool tst_string_util_trim();
bool tst_string_util_split();
bool tst_url_encode();
bool tst_url_decode();

bool tst_to_lower();

bool tst_starts_with();
bool tst_ends_with();

int main() {
    bool all_ok = tst_string_util_split() && tst_string_util_trim() &&
                  tst_url_encode() && tst_url_decode() && tst_to_lower() &&
                  tst_starts_with() && tst_ends_with();

    return all_ok ? 0 : -1;
}
