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

namespace FurryBear\Common\Validation;

use FurryBear\Common\Message\Group as MessageGroup,
    FurryBear\Common\Message\Message as ValidationMessage,
    FurryBear\Common\Exception\InvalidArgumentException;

/**
 * Allows to validate data using validators
 * 
 * @category Congress_API
 * @package  FurryBear
 * @author   lobostome <lobostome@local.dev>
 * @license  http://opensource.org/licenses/MIT MIT License
 * @link     https://github.com/lobostome/FurryBear
 */

class Engine
{
    /**
     * The data to be verified.
     * 
     * @var array 
     */
    protected $data = array();
    
    /**
     * A container for registered validators.
     * 
     * @var array 
     */
    protected $validators = array();
    
    /**
     * A container for a group of messages.
     * 
     * @var \FurryBear\Message\Group 
     */
    protected $messages;
    
    /**
     * Construct the engine and run init function if available.
     */
    public function __construct()
    {
        if (method_exists($this, 'init')) {
            $this->init();
        }
        
        $this->messages = new MessageGroup();
    }
    
    /**
     * Performs the validation.
     * 
     * @return boolean
     * 
     * @throws InvalidArgumentException
     */
    public function isValid()
    {        
        if (empty($this->validators)) {
            throw new InvalidArgumentException("There are no validators specified.");
        }
        
        if (empty($this->data)) {
            throw new InvalidArgumentException("There is no data to validate.");
        }
        
        foreach ($this->data as $key => $value) {
            if (isset($this->validators[$key])) {
                foreach ($this->validators[$key] as $validator) {
                    $validator->setValue($value);
                                        
                    if (!$validator->validate()) {
                        $this->messages[] = new ValidationMessage($validator->getOption('message'));
                    }
                }
            }
        }
        
        if (count($this->messages) > 0) {
            return FALSE;
        }
        
        return TRUE;
    }
    
    /**
     * Adds a validator to the container.
     * 
     * @param string                                 $attribute
     * @param \FurryBear\Common\Validation\Validator $validator
     * 
     * @return void
     */
    public function add($attribute, Validator $validator)
    {
        $this->validators[$attribute][] = $validator;
    }
    
    /**
     * Remove a validator/validators
     * 
     * @param string $attribute
     * @param string $validator
     */
    public function remove($attribute, $validator = null)
    {
        if (isset($this->validators[$attribute])) {
            
            if (is_null($validator)) {
                unset($this->validators[$attribute]);
                return;
            }
            
            foreach ($this->validators[$attribute] as $k => $v) {
                if (get_class($v) == $validator) {
                    unset($this->validators[$attribute][$k]);
                    break;
                }
            }
        }
    }
    
    /**
     * Sets the data to be validated.
     * 
     * @param array $data
     * 
     * @return void
     */
    public function populate($data)
    {
        $this->data = $data;
    }
    
    /**
     * Gets failed validation messages.
     * 
     * @return \FurryBear\Message\Group
     */
    public function getMessages()
    {
        return $this->messages;
    }
}