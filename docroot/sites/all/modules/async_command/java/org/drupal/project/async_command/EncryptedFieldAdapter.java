package org.drupal.project.async_command;

import org.apache.commons.codec.binary.Base64;
import org.drupal.project.async_command.exception.DrupletException;

import java.util.Properties;
import java.util.logging.Logger;

/**
 * This class handles encryption through the {@see <a href="http://drupal.org/project/encset">Encrypted Field Drupal Module</a>}.
 * Encryption is used for confidential information.
 * TODO: use Java encryption/decryption library rather than resort to PHP evaluation.
 */
public class EncryptedFieldAdapter {

    private static Logger logger = DrupletUtils.getPackageLogger();

    /**
     * Encryption method.
     */
    public static enum Method {
        /**
         * Not encrypted.
         */
        NONE,

        /**
         * Encrypted using base64 algorithm. No secret key involded.
         */
        BASE64,

        /**
         * Encrypted using the MCRYPT method. {@see http://php.net/manual/en/book.mcrypt.php}
         */
        MCRYPT
    }

    /**
     * The encryption method of this object.
     */
    private final Method method;

    /**
     * If the method is mcrypt, this is the key for encryption/decryption.
     */
    private final String secretkey;

    /**
     * Initialize this adapter.
     *
     * @param method {@see EncryptionMethod}
     */
    public EncryptedFieldAdapter(Method method) {
        this.method = method;
        this.secretkey = null;
    }

    /**
     * Initialize this adapter.
     *
     * @param method {@see EncryptionMethod}
     * @param secretKey The secret key.
     */
    public EncryptedFieldAdapter(Method method, String secretKey) {
        this.method = method;
        this.secretkey = secretKey;
    }

    /**
     * Read the original content of the encrypted message.
     * @param encrypted Encrypted message to be decrypted.
     * @return Decrypted orignal message.
     */
    public String readContent(String encrypted) {
        String original = null;
        switch (method) {
            case NONE:
                original = encrypted;
                break;
            case BASE64:
                original = new String(Base64.decodeBase64(encrypted));
                break;
            case MCRYPT:
                if (secretkey == null) {
                    throw new DrupletException("Need to set secretKey in order to decrypt MCRYPT message.");
                }
                original = DrupletUtils.evalPhp("echo rtrim(mcrypt_decrypt(MCRYPT_3DES,''{0}'',base64_decode(''{1}''),''ecb''),''\\0'');", this.secretkey, encrypted);
                original = original.trim();
                break;
        }
        return original;
    }

    public Properties readSettings(String encrypted) {
        String original = readContent(encrypted);
        return DrupletUtils.loadProperties(original);
    }

}
