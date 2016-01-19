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

namespace FurryBear\Output\Strategy;

use FurryBear\Common\Exception\InvalidJsonException,
    FurryBear\Output\Strategy,
    FurryBear\Output\Strategy\Error\Json as JsonError;

/**
 * Converts the json string returned from the service to an object.
 * 
 * @category Congress_API
 * @package  FurryBear
 * @author   lobostome <lobostome@local.dev>
 * @license  http://opensource.org/licenses/MIT MIT License
 * @link     http://sunlightlabs.github.com/congress/
 */
class JsonToObject implements Strategy
{
    /**
     * Convert the json data to an object.
     * 
     * @param string $data The json string returned from the service.
     * 
     * @return object
     * @throws \FurryBear\Common\Exception\InvalidJsonException
     */
    public function convert($data)
    {
        $obj = json_decode($data);
        
        if (json_last_error() == JSON_ERROR_NONE) {
            return $obj;
        } else {
            $error = new JsonError(json_last_error());
            throw new InvalidJsonException(__CLASS__ . ' Output Strategy Report: ' . $error);
        }
    }
}