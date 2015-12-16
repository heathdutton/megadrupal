<?php

class phpbb_sso_hash_salted_md5 extends phpbb_sso_hash
{
    public static function hash($password) {
        $random = parent::get_random_salt(6);
        $hash = self::hash_crypt($password, self::generate_salt($random));

        if (strlen($hash) == 34) {
            return $hash;
        }

        return md5($password);
    }

    /**
     * The crypt function/replacement
     */
    public static function hash_crypt($password, $setting) {
        $output = '*';

        // Check for correct hash
        if (substr($setting, 0, 3) != '$H$' && substr($setting, 0, 3) != '$P$') {
            return $output;
        }

        $count_log2 = strpos(parent::itoa64, $setting[3]);

        if ($count_log2 < 7 || $count_log2 > 30) {
            return $output;
        }

        $count = 1 << $count_log2;
        $salt = substr($setting, 4, 8);

        if (strlen($salt) != 8) {
            return $output;
        }

        $hash = md5($salt . $password, true);
        do {
            $hash = md5($hash . $password, true);
        } while (--$count);

        $output = substr($setting, 0, 12);
        $output .= parent::hash_encode64($hash, 16);

        return $output;
    }

    public static function check($password, $hash) {
        if (strlen($hash) !== 34) {
            return md5($password) === $hash;
        }

        return parent::string_compare($hash, self::hash_crypt($password, $hash));
    }

    public static function generate_salt($input, $iteration_count_log2 = 6) {
        if ($iteration_count_log2 < 4 || $iteration_count_log2 > 31) {
            $iteration_count_log2 = 8;
        }

        $output = '$H$';
        $output .= parent::itoa64[min($iteration_count_log2 + 5, 30)];
        $output .= parent::hash_encode64($input, 6);

        return $output;
    }
}
