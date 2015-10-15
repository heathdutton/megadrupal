<?php
/**
 * Created by ConfidentTechnologies.
 * User: byao@confidenttech.com
 * Date: 8/26/12
 * Time: 4:11 PM
 * PHP ConfidentCaptcha Implementation
 */
/**
 * Confident CAPTCHA Properties Class
 * @package confidentcaptcha-php
 */
class ConfidentCaptchaProperties
{
    /**
     * @var  An array to store the CAPTCHA properties
     */
    private $properties;


    /**
     * Constructor
     */
    public function __construct($settingsXml = null)
    {
        if ($settingsXml != null) {
            $xml = simplexml_load_file($settingsXml);
            $properties = $xml->{'properties'};

            if (isset($properties)) {
                $json = json_encode($properties);
                $this->properties = json_decode($json, true);
            }
        }

        if (is_null($this->properties))
        {
            $this->setDefaultProperties();
        }

    }


    /**
     * Getter for the array holding CAPTCHA properties
     *
     * @return
     */
    public function getProperties()
    {
        if (is_null($this->properties))
        {
            $this->setDefaultProperties();
        }

        return $this->properties;
    }

    /**
     * Setter for the array to hold CAPTCHA properties
     *
     * @param $properties
     */
    public function setProperties($properties)
    {
        $this->properties = $properties;
    }

    private function setDefaultProperties()
    {
        $this->properties['width'] = '3';
        $this->properties['height'] = '3';
        $this->properties['captcha_length'] = '4';
        $this->properties['display_style'] = 'lightbox';
        $this->properties['image_code_color'] = 'White';
        $this->properties['include_audio_form'] = 'FALSE';
        $this->properties['noise_level'] = '0.1';
        $this->properties['failure_policy_math'] = 'math';		
    }

    public function getProperty($propertyName)
    {
        return $this->properties[$propertyName];
    }

    public function setProperty($propertyName, $propertyValue)
    {
        $this->properties[$propertyName] = $propertyValue;
    }

    // getters and setters provided for known properties
    public function setBannerName($banner_name)
    {
        $this->properties['banner_name'] = $banner_name;
    }

    public function getBannerName()
    {
        return $this->properties['banner_name'];
    }

    public function setCaptchaLength($captcha_length)
    {
        $this->properties['captcha_length'] = $captcha_length;
    }

    public function getCaptchaLength()
    {
        return $this->properties['captcha_length'];
    }

    public function setDisplayStyle($display_style)
    {
        $this->properties['display_style'] = $display_style;
    }

    public function getDisplayStyle()
    {
        return $this->properties['display_style'];
    }

    public function setHeight($height)
    {
        $this->properties['height'] = $height;
    }

    public function getHeight()
    {
        return $this->properties['height'];
    }

    public function setImageCodeColor($image_code_color)
    {
        $this->properties['image_code_color'] = $image_code_color;
    }

    public function getImageCodeColor()
    {
        return $this->properties['image_code_color'];
    }

    public function setLogoName($logo_name)
    {
        $this->properties['logo_name'] = $logo_name;
    }

    public function getLogoName()
    {
        return  $this->properties['logo_name'];
    }

    public function setNoiseLevel($noise_level)
    {
        $this->properties['noise_level'] = $noise_level;
    }

    public function getNoiseLevel()
    {
        return $this->properties['noise_level'];
    }

    public function setWidth($width)
    {
        $this->properties['width'] = $width;
    }

    public function getWidth()
    {
        return $this->properties['width'];
    }

    public function setApiServerUrl(){
        $this->properties['api_server_url'];
    }

    public function getApiServerUrl(){
        return $this->properties['api_server_url'];
    }

    public function setLibraryVersion($library_version)
    {
        $this->properties['library_version']  = $library_version;
    }

    public function getLibraryVersion()
    {
        return $this->properties['library_version'];
    }
}