<?php

class AES
{
    const M_CBC = 'cbc';
    const M_CFB = 'cfb';
    const M_ECB = 'ecb';
    const M_NOFB = 'nofb';
    const M_OFB = 'ofb';
    const M_STREAM = 'stream';

    protected $key;

    protected $cipher;

    protected $data;

    protected $mode;

    protected $IV;

    /**
     * Sets all the required data.
     * @param string $data
     * @param string $key
     * @param int $blockSize
     * @param string $mode
     * @param string $iv
     */
    function __construct($data = null, $key = null, $blockSize = null, $mode = null, $iv)
    {
        $this->setData($data);
        $this->setKey($key);
        $this->setBlockSize($blockSize);
        $this->setMode($mode);
        $this->setIV($iv);
    }

    /**
     * @param string $data
     */
    public function setData($data)
    {
        $this->data = $data;
    }

    /**
     * @param string $key
     */
    public function setKey($key)
    {
        $this->key = $key;
    }

    /**
     * @param string $IV
     */
    public function setIV($IV)
    {
        $this->IV = $IV;
    }

    /**
     * @return string $IV
     */
    protected function getIV()
    {
        if ($this->IV == "") {
            $this->IV = mcrypt_create_iv(
                mcrypt_get_iv_size($this->cipher, $this->mode), MCRYPT_RAND);
        }
        return $this->IV;
    }

    /**
     * @param int $blockSize
     */
    public function setBlockSize($blockSize)
    {
        switch ($blockSize) {
            case 128 :
                $this->cipher = MCRYPT_RIJNDAEL_128;
                break;

            case 192 :
                $this->cipher = MCRYPT_RIJNDAEL_192;
                break;

            case 256 :
                $this->cipher = MCRYPT_RIJNDAEL_256;
                break;
        }
    }

    /**
     * @param string $mode
     */
    public function setMode($mode)
    {

        switch ($mode) {
            case AES::M_CBC :
                $this->mode = MCRYPT_MODE_CBC;
                break;
            case AES::M_CFB :
                $this->mode = MCRYPT_MODE_CFB;
                break;
            case AES::M_ECB :
                $this->mode = MCRYPT_MODE_ECB;
                break;
            case AES::M_NOFB :
                $this->mode = MCRYPT_MODE_NOFB;
                break;
            case AES::M_OFB :
                $this->mode = MCRYPT_MODE_OFB;
                break;
            case AES::M_STREAM :
                $this->mode = MCRYPT_MODE_STREAM;
                break;
            default :
                $this->mode = MCRYPT_MODE_ECB;
                break;
        }
    }

    /**
     * This function checks whether the mandatory values are set and returns true. Returns false otherwise.
     * @return boolean
     */
    public function validateParams()
    {
        if ($this->data != null && $this->key != null && $this->cipher != null) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * This function encrypts the data according to the key, mode and iv provided
     * and convert it into base64 encoded format
     * @return string
     * @throws Exception
     */
	public function encrypt()
    {
        if ($this->validateParams()) {
            $str = $this->pad($this->data);
            $td = mcrypt_module_open($this->cipher, '', $this->mode, '');

            if (empty($this->IV)) {
                $iv = @mcrypt_create_iv(mcrypt_enc_get_iv_size($td),
                    MCRYPT_RAND);
            } else {
                $iv = $this->IV;
            }

            mcrypt_generic_init($td, $this->key, $iv);
            $cyper_text = mcrypt_generic($td, $str);
            $rt = base64_encode($cyper_text);
            mcrypt_generic_deinit($td);
            mcrypt_module_close($td);
            return $rt;
        } else {
            throw new Exception(
                'Provide valid details to get transaction token');
        }
    }

    /**
     * This function decrypts the data according to the key, mode and iv provided
     * @return string
     * @throws Exception
     */
	public function decrypt()
    {
        $td = mcrypt_module_open($this->cipher, '', $this->mode, '');

        if (empty($this->IV)) {
            $iv = @mcrypt_create_iv(mcrypt_enc_get_iv_size($td), MCRYPT_RAND);
        } else {
            $iv = $this->IV;
        }

        mcrypt_generic_init($td, $this->key, $iv);
        $decrypted_text = mdecrypt_generic($td, base64_decode($this->data));
        $rt = $decrypted_text;
        mcrypt_generic_deinit($td);
        mcrypt_module_close($td);

        return $this->unpad($rt);
    }

    /**
     * This function encrypts the data according to the key, mode and iv provided
     * and convert it into hex encoded format
     * @see http://in2.php.net/manual/en/function.bin2hex.php
     * @return string
     * @throws Exception
     */
	public function encryptHex()
    {
        $str = $this->pad($this->data);
        $td = mcrypt_module_open($this->cipher, '', $this->mode, '');

        if (empty($this->IV)) {
            $iv = @mcrypt_create_iv(mcrypt_enc_get_iv_size($td), MCRYPT_RAND);
        } else {
            $iv = $this->IV;
        }

        mcrypt_generic_init($td, $this->key, $iv);
        $cyper_text = mcrypt_generic($td, $str);
        $rt = bin2hex($cyper_text);
        mcrypt_generic_deinit($td);
        mcrypt_module_close($td);

        return $rt;
    }

    /**
     * This function decrypts the data according to the key, mode and iv provided
     * @return string
     * @throws Exception
     */
	public function decryptHex()
    {
        $td = mcrypt_module_open($this->cipher, '', $this->mode, '');

        if (empty($this->IV)) {
            $iv = @mcrypt_create_iv(mcrypt_enc_get_iv_size($td), MCRYPT_RAND);
        } else {
            $iv = $this->IV;
        }

        mcrypt_generic_init($td, $this->key, $iv);
        $decrypted_text = mdecrypt_generic($td, self::hex2bin($this->data));
        $rt = $decrypted_text;
        mcrypt_generic_deinit($td);
        mcrypt_module_close($td);

        return $this->unpad($rt);
    }

    /**
     * This function converts hex value to string
     * @return string
     */
	public static function hex2bin($hexdata) {
        $bindata = '';
        $length = strlen($hexdata);
        for ($i=0; $i < $length; $i += 2)
        {
            $bindata .= chr(hexdec(substr($hexdata, $i, 2)));
        }
        return $bindata;
    }

	/**
	 * Sets pad_method
	 */
	public function require_pkcs5()
    {
        $this->pad_method = 'pkcs5';
    }

    /**
     * @param string $str
     * @param string  $ext
     * @return string
     */
    protected function pad_or_unpad($str, $ext)
    {
        if (is_null($this->pad_method))
        {
            return $str;
        }
        else
        {
            $func_name = __CLASS__ . '::' . $this->pad_method . '_' . $ext . 'pad';
			if (is_callable($func_name))
            {
                $size = mcrypt_get_block_size($this->cipher, $this->mode);
                return call_user_func($func_name, $str, $size);
            }
        }

        return $str;
    }

    /**
     * @param string $str
     * @return string
     */
    protected function pad($str)
    {
        return $this->pad_or_unpad($str, '');
    }

    /**
     * @param string $str
     * @return string
     */
    protected function unpad($str)
    {
        return $this->pad_or_unpad($str, 'un');
    }

	/**
	 * @param string $text
	 * @param int $blocksize
	 * @return string
	 */
	public static function pkcs5_pad($text, $blocksize)
    {
        $pad = $blocksize - (strlen($text) % $blocksize);
        return $text . str_repeat(chr($pad), $pad);
    }

    /**
     * @param string $text
     * @return string
     */
    public static function pkcs5_unpad($text)
    {
        $pad = ord($text{strlen($text) - 1});
        if ($pad > strlen($text)) return false;
        if (strspn($text, chr($pad), strlen($text) - $pad) != $pad) return false;
        return substr($text, 0, -1 * $pad);
    }

}