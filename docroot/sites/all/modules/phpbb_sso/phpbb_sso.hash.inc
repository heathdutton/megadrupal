<?php

class phpbb_sso_hash {
    const itoa64 = './0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';

    /**
    * Encode hash.
    */
    public static function hash_encode64($input, $count) {
        $output = '';
        $i = 0;

        do {
            $value = ord($input[$i++]);
            $output .= self::itoa64[$value & 0x3f];

            if ($i < $count) {
                $value |= ord($input[$i]) << 8;
            }

            $output .= self::itoa64[($value >> 6) & 0x3f];

            if ($i++ >= $count) {
                break;
            }

            if ($i < $count) {
                $value |= ord($input[$i]) << 16;
            }

            $output .= self::itoa64[($value >> 12) & 0x3f];

            if ($i++ >= $count) {
                break;
            }

            $output .= self::itoa64[($value >> 18) & 0x3f];
        } while ($i < $count);

        return $output;
    }

    /**
     * Get random salt with specified length
     *
     * @param int $length Salt length
     * @param string $rand_seed Seed for random data (optional). For tests.
     *
     * @return string Random salt with specified length
     */
    public static function get_random_salt($length, $rand_seed = '/dev/urandom') {
        $random = '';

        if (($fh = @fopen($rand_seed, 'rb'))) {
            $random = fread($fh, $length);
            fclose($fh);
        }

        if (strlen($random) < $length) {
            module_load_include('inc', 'phpbb_sso', 'phpbb_sso.utility');

            $random = '';
            $random_state = phpbb_sso_unique_id();

            for ($i = 0; $i < $length; $i += 16) {
                $random_state = md5(phpbb_sso_unique_id() . $random_state);
                $random .= pack('H*', md5($random_state));
            }
            $random = substr($random, 0, $length);
        }

        return $random;
    }

    /**
     * Compare two strings byte by byte
     *
     * @param string $string_a The first string
     * @param string $string_b The second string
     *
     * @return bool True if strings are the same, false if not
     */
    public static function string_compare($string_a, $string_b) {
        // Return if input variables are not strings or if length does not match
        if (!is_string($string_a) || !is_string($string_b) || strlen($string_a) != strlen($string_b)) {
            return false;
        }

        // Use hash_equals() if it's available
        if (function_exists('hash_equals')) {
            return hash_equals($string_a, $string_b);
        }

        $difference = 0;

        for ($i = 0; $i < strlen($string_a) && $i < strlen($string_b); $i++) {
            $difference |= ord($string_a[$i]) ^ ord($string_b[$i]);
        }

        return $difference === 0;
    }
}

class phpbb_sso_hash_manager {
    private $hash;

    public function __construct($hash) {
        if (in_array(substr($hash, 0, 4), array('$2a$', '$2x$', '$2y$'))) {
            module_load_include('inc', 'phpbb_sso', 'phpbb_sso.hash_bcrypt');

            $this->hash = new phpbb_sso_hash_bcrypt();
        } else if (in_array(substr($hash, 0, 3), array('$H$', '$P$'))) {
            module_load_include('inc', 'phpbb_sso', 'phpbb_sso.hash_md5');

            $this->hash = new phpbb_sso_hash_salted_md5();
        }
    }

    /**
     * Check for correct password
     *
     * @param string $password The password in plain text
     * @param string $hash The stored password hash
     *
     * @return bool Returns true if the password is correct, false if not.
     */
    public function check_hash($password, $hash) {
        return $this->hash->check($password, $hash);
    }

    public function hash($password) {
        return $this->hash->hash($password);
    }
}
