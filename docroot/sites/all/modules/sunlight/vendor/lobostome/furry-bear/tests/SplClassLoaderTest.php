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

/**
 * Test for autoloader.
 * 
 * @category Congress_API
 * @package  FurryBear
 * @author   lobostome <lobostome@local.dev>
 * @license  http://opensource.org/licenses/MIT MIT License
 * @link     https://github.com/lobostome/FurryBear
 */
class SplClassLoaderTest extends PHPUnit_Framework_TestCase {
    
    /**
     * A reference to the SplClassLoader object.
     * 
     * @var \SplClassLoader
     */
    protected $autoload;
    
    /**
     * Create fixtures.
     */
    protected function setUp()
    {
        require_once __DIR__ . '/../SplClassLoader.php';
        $this->autoload = new SplClassLoader(__DIR__ . '/../src');
    }
    
    /**
     * Test to ensure the loader autoloads the needed classes.
     */
    public function testClassLoad()
    {
        $this->assertInstanceOf('Furrybear\FurryBear', 
                                new \FurryBear\FurryBear());
    }
    
    /**
     * Test to ensure the autoload function is registered.
     */
    public function testRegister()
    {
        $this->autoload->register();
        
        $spl_autoload_functions = spl_autoload_functions();
        
        foreach($spl_autoload_functions as $f) {
            if(is_array($f) && $f[0] instanceof SplClassLoader) {
                $this->assertEquals($f[1], 'furrybear_autoload');
            }
        }
    }
    
    /**
     * Test file not found.
     */
    public function testFileDoesNotExistException()
    {
        $this->setExpectedException('\\FurryBear\\Common\\Exception\\FileDoesNotExistException');
        new \FurryBear\Something();
    }
    
    /**
     * Clean up fixtures.
     */
    protected function tearDown()
    {
        spl_autoload_unregister(array('SplClassLoader', 'furrybear_autoload'));
    }
}