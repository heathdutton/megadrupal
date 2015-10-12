<?php

namespace Drupal\go1_base\Tests\Unit;

use Drupal\go1_base\Helper\Test\UnitTestCase;

class ServiceContentRenderTest extends UnitTestCase {
  public function getInfo() {
    return array('name' => 'GO1 Unit: Test helper.content_render service') + parent::getInfo();
  }

  public function setUp() {
    parent::setUp();
    $this->render = go1_container('helper.content_render');
  }

  public function testString() {
    $string = 'Hello GO1';
    $output = $this->render->render($string);
    $this->assertEqual($string, $output);
  }

  public function testFunction() {
    $output = $this->render->render(array('function' => 'go1test_base_hello'));
    $this->assertEqual('Hello GO1', $output);
  }

  public function testStaticMethod() {
    $output = $this->render->render(array('function' => '\Go1_Base_Test_Class::helloStatic'));
    $this->assertEqual('Hello GO1', $output);
  }

  public function testTemplateString() {
    $data = array();
    $data['template_string'] = 'Hello {{ name }}';
    $data['variables']['name'] = 'GO1';
    $output = $this->render->render($data);
    $this->assertEqual('Hello GO1', $output);
  }
  public function testTemplateStringEmptyReturn() {
    $data = array();
    $data['template_string'] = '{% if false %}Empty return{% endif %}';
    $output = $this->render->render($data);
    $this->assertEqual('', $output);
  }

  public function testTemplate() {
    $data = array();
    $data['template'] = '@go1test_base/templates/block/hello_template.html.twig';
    $data['variables']['name'] = 'GO1';
    $output = trim($this->render->render($data));
    $this->assertEqual('Hello GO1', $output);
  }

  public function testDynamicVariables() {
    $data = array();

    $expected = 'Hello GO1';
    $data['template_string'] = 'Hello {{ name }}';

    // Function
    $data['variables'] = 'go1test_variables';
    $this->assertEqual($expected, $this->render->render($data));

    // Static call
    $data['variables'] = 'Go1_Base_Test_Class::staticGetVariables';
    $this->assertEqual($expected, $this->render->render($data));

    // object/method
    $obj = new \Go1_Base_Test_Class();
    $data['variables'] = array($obj, 'getVariables');
    $this->assertEqual($expected, $this->render->render($data));

    // Class/Method
    $data['variables'] = array('Go1_Base_Test_Class', 'staticGetVariables');
    $this->assertEqual($expected, $this->render->render($data));

    // getVariables method of controller class
    unset($data['variables']);
    $data['controller'] = array('Go1_Base_Test_Class', 'hi');
    $this->assertEqual($expected, $this->render->render($data));
  }
}
