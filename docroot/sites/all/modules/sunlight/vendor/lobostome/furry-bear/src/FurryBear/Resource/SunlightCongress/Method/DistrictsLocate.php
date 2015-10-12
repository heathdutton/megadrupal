<?php

/**
 * FurryBear
 * 
 * PHP Version 5.3
 * 
 * @category Congress_API
 * @package  FurryBear
 * @author   lobostome <lobostome@local.dev>
 * @license  http://opensource.org/licenses/MIT MIT License
 * @link     https://github.com/lobostome/FurryBear
 */

namespace FurryBear\Resource\SunlightCongress\Method;

use FurryBear\Resource\SunlightCongress\BaseResource;

/**
 * This class gives access to Sunlight Congress districts/locate resource.
 * 
 * @category Congress_API
 * @package  FurryBear
 * @author   lobostome <lobostome@local.dev>
 * @license  http://opensource.org/licenses/MIT MIT License
 * @link     https://github.com/lobostome/FurryBear
 */
class DistrictsLocate extends BaseResource
{
    /**
     * The resource method URL. No slashes at the beginning and end of the 
     * string.
     */
    const ENDPOINT_METHOD = 'districts/locate';

    /**
     * Constructs the resource, sets a reference to the FurryBear object, and 
     * sets the resource method URL.
     * 
     * @param \FurryBear\FurryBear $furryBear A reference to the FurryBear onject.
     */
    public function __construct(\FurryBear\FurryBear $furryBear)
    {
        parent::__construct($furryBear);
        $this->setResourceMethod(self::ENDPOINT_METHOD);
    }
    
    /**
     * Retrieves districts based on a zip
     * 
     * @param string $zip
     * 
     * @return mixed
     */
    public function getByZip($zip)
    {
        $params = array('zip' => (string)$zip);
        return $this->get($params);
    }
    
    /**
     * Retrieves districts based on an address
     * 
     * @param string $address The target address
     * 
     * @return mixed
     */
    public function getByAddress($address)
    {
        if (is_null($this->geocodeProvider)) {
            throw new \Exception('No geocode provider specified. To set up one, use the via() method');
        }
        $params = $this->geocodeProvider->geocode($address);
        return $this->get($params);
    }
    
    /**
     * Set the geocode provider and its adapter and output strategy.
     * 
     * @param \FurryBear\Geocode\AbstractProvider $provider The geocode provider
     * 
     * @return \FurryBear\Resource\SunlightCongress\LegislatorsLocate
     */
    public function via(\FurryBear\Geocode\AbstractProvider $provider)
    {
        $this->geocodeProvider = $provider;
        $this->geocodeProvider->setAdapter($this->furryBear->getProvider()->getAdapter());
        
        return $this;
    }
}