<?php

class phpbb_sso_hash_bcrypt extends phpbb_sso_hash {
    public static function hash($password, $salt = '') {
        if ($salt == '') {
            $prefix = '$2y$';
            $salt = $prefix . '10$' . self::generate_salt();
        }

        $hash = crypt($password, $salt);
        if (strlen($hash) < 60) {
            return false;
        }

        return $hash;
    }

    public static function check($password, $hash) {
        $salt = substr($hash, 0, 29);
        if (strlen($salt) != 29) {
            return false;
        }

        if (parent::string_compare($hash, self::hash($password, $salt))) {
            return true;
        }

        return false;
    }

    public static function generate_salt() {
        return parent::hash_encode64(parent::get_random_salt(22), 22);
    }
}
