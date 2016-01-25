<?php
require('../AppleNews.php'); 

class AppleNewsTest extends \PHPUnit_Framework_TestCase
{
	public function setUp() {
    date_default_timezone_set('UTC');
    $settings = json_decode(file_get_contents('./settings.json'), true);
    $this->instance = new AppleNews($settings['api_key'], $settings['api_secret']);
    $this->channel_id = $settings['api_channel_id'];
  }

  public function tearDown() {
    unset($this->instance);
    unset($this->channel_id);
  }
    
	public function testGetChannel() {

    $channel = $this->instance->getChannel($this->channel_id);
    
    $this->assertEquals($channel['id'], $this->channel_id);
    $this->assertTrue(is_string($channel['createdAt']));
    $this->assertTrue(is_string($channel['modifiedAt']));
    $this->assertTrue(is_string($channel['id']));
    $this->assertEquals($channel['type'], 'channel');
    
    $this->assertTrue(is_array($channel['links']));
    $this->assertTrue(is_string($channel['links']['defaultSection']));
    $this->assertTrue(is_string($channel['links']['self']));
    $this->assertTrue(is_string($channel['name']));
    $this->assertTrue(is_string($channel['website']));
	}
	
	public function exampleArticlesProvider()
  {
      return array(
        
        array(
          './Example_Articles/Simple/Simple7Column/article.json',
          array(),
        ),
        array(
          './Example_Articles/Simple/SimpleRightMargin5Column/article.json',
          array(),
        ),
        array(
          './Example_Articles/Example_Recipe_Article/article.json',
          array(
            './Example_Articles/Example_Recipe_Article/bodyImage1.jpg',
            './Example_Articles/Example_Recipe_Article/header.jpg'
          ),
        ),
        array(
          './Example_Articles/Example_News_Article/article.json',
          array(
            './Example_Articles/Example_News_Article/bodyGraphic.png',
            './Example_Articles/Example_News_Article/galleryImage1.jpg',
            './Example_Articles/Example_News_Article/galleryImage2.jpg',
            './Example_Articles/Example_News_Article/galleryImage3.jpg',
            './Example_Articles/Example_News_Article/header.jpg',
          ),
        ),
      );
  }
	
	/**
   * @dataProvider exampleArticlesProvider
   */
	public function testArticleCalls($article_path, $files) {

  	//POST
  	$json_string = file_get_contents($article_path);
  	$article = $this->instance->postArticle($this->channel_id, $json_string, $files);
  	$this->assertTrue(is_string($article['id']));
  	
  	//GET
  	$article_get = $this->instance->getArticle($article['id']);
  	$this->assertTrue(is_string($article_get['id']));
  	$this->assertEquals($article_get['id'], $article['id']);
  	
  	$this->assertTrue(is_string($article_get['createdAt']));
  	$this->assertTrue(is_string($article_get['modifiedAt']));
  	$this->assertTrue(is_string($article_get['id']));
  	
  	$this->assertTrue(is_array($article_get['links']));
  	$this->assertTrue(is_string($article_get['links']['self']));
  	$this->assertTrue(is_string($article_get['links']['channel']));
  	$this->assertTrue(is_array($article_get['links']['sections']));
  
  	$this->assertTrue(is_array($article_get['document']));
  	$this->assertTrue(is_bool($article_get['isCandidateToBeFeatured']));
  	$this->assertTrue(is_bool($article_get['isSponsored']));
  	$this->assertTrue(is_string($article_get['shareUrl']));
  	
  	//DELETE
  	$this->instance->deleteArticle($article_get['id']);
	}
	
	public function exampleInvalidArticlesProvider() {
  	return array(
    	array('{}'),
    	array('{"title": "Simple 7 Columns"}'),
    	array('invalid string')
  	);
	}
	
	/**
   * @dataProvider exampleInvalidArticlesProvider
   */
	public function testInvalidArticleCalls($json_string) {
  	$expected_error = 'The JSON in the Native document (article.json) is invalid. Please test with the Preview Tool first.';
  	$returned_error = '';
  	try {
    	$article = $this->instance->postArticle($this->channel_id, $json_string);
  	}
  	catch (Exception $e) {
      $returned_error = $e->getMessage();
    }
  	$this->assertEquals($expected_error, $returned_error);
	}
	
	public function testSectionsCalls() {

  	$sections = $this->instance->getChannelSections($this->channel_id);
  	$this->assertTrue(is_array($sections));
  	
  	foreach($sections as $section) {
    	$this->assertTrue(is_string($section['name']));
    	$this->assertTrue(is_array($section['links']));
    	$this->assertTrue(is_string($section['links']['self']));
    	$this->assertTrue(is_string($section['links']['channel']));
    	
    	$this->assertTrue(is_string($section['modifiedAt']));
    	$this->assertTrue(is_bool($section['isDefault']));
    	$this->assertTrue(is_string($section['type']));
    	$this->assertEquals($section['type'], 'section');
    	$this->assertTrue(is_string($section['id']));
    	
    	$section_get = $this->instance->getSection($section['id']);
    	$this->assertEquals($section['id'], $section_get['id']);
  	}
	}

}