<?php

if (!class_exists('dcBase')) {
  require_once('class.dcbase.inc');
}

/**
 * Text class definition.
 */
class dcText extends dcBase implements dcContentInterface
{
  /***************************************************
       DATA
  ***************************************************/
  
  private $type_options = array(
   'word',
   'sentence',
   'paragraph',
  );
  
  private $data = array();
  
  private $size = 0;
  
  private $content_db = array(
    'dc_random'  => array(
      'Lorem ipsum dolor sit amet, consectetur adipiscing elit.',
      'Sed nec est et velit dictum dapibus a sit amet nulla.',
      'Ut in enim vitae nulla sollicitudin eleifend.',
      'Vestibulum in nisl tortor, ac varius risus.',
      'Vivamus vulputate fermentum arcu, congue molestie libero congue et.',
      'Nam venenatis sodales est, vel fermentum ipsum scelerisque vel.',
      'Nunc ut arcu diam, pharetra faucibus elit.',
      'Ut ac felis quis nisl laoreet adipiscing ac a eros.',
      'Aliquam varius mattis diam, vitae feugiat ligula eleifend a.',
      'Praesent sed urna in metus suscipit venenatis.',
      'Proin sodales placerat turpis, accumsan vulputate magna eleifend ut.',
      'Mauris tempus iaculis mi, eget molestie lorem imperdiet in.',
      'Aliquam lobortis viverra dui, sit amet rhoncus eros mollis sit amet.',
      'Mauris sodales metus vitae diam dapibus nec congue erat condimentum.',
      'Proin in velit ac augue accumsan dapibus.',
      'Phasellus sagittis massa nec turpis posuere in malesuada dolor vulputate.',
      'Quisque euismod nunc semper augue elementum quis viverra ipsum facilisis.',
      'Aenean in ipsum enim, ut lobortis nisi.',
      'In hendrerit tortor vel augue hendrerit non luctus augue accumsan.',
      'Quisque posuere placerat purus, eget malesuada nulla ornare in.',
      'Nulla eget dolor sed nunc viverra rhoncus.',
      'Vivamus in lorem justo, non fermentum libero.',
      'Proin quis tellus at velit lobortis consequat.',
      'Cras vel tellus nisi, vel vestibulum diam.',
      'Suspendisse eu metus felis, eget semper dolor.',
      'Sed vel ipsum id odio aliquet ultrices.',
      'Mauris cursus vestibulum nunc, non accumsan purus auctor et.',
      'In sagittis magna eu leo fermentum mattis quis at enim.',
      'Phasellus vitae leo varius dui auctor suscipit.',
      'Maecenas vel magna at nisi mollis varius in vel est.',
      'Nunc mattis convallis mi, vel gravida magna pretium at.',
      'Mauris tristique est eu ante euismod posuere.',
      'Cras eu felis eget erat placerat ultricies et a est.',
      'Vestibulum tristique nunc sed tortor viverra a vehicula sapien volutpat.',
      'Cras eu risus a enim lacinia eleifend.',
      'Donec et sem ligula, et interdum enim.',
      'In cursus enim sit amet lectus tristique in semper arcu rhoncus.',
      'Sed aliquet odio at neque egestas blandit.',
      'Proin posuere neque id nibh auctor sit amet rhoncus purus fringilla.',
      'Phasellus sit amet justo ante, quis egestas dui.',
      'Pellentesque tempus leo ut tortor sodales interdum.',
      'Nunc sodales interdum elit, aliquet luctus mauris pretium quis.',
      'Integer viverra ante volutpat dolor accumsan porttitor.',
      'Vestibulum congue risus ac neque iaculis rutrum semper erat dictum.',
      'Pellentesque ornare sem sed dui adipiscing pulvinar.',
      'Morbi id arcu ut dui rhoncus vestibulum.',
      'Pellentesque in sem est, ut fringilla eros.',
      'Cras feugiat congue justo, nec accumsan lacus lobortis a.',
      'Integer ac arcu sem, nec accumsan lacus.',
      'Donec ac elit ac eros lobortis auctor.',
      'Quisque dapibus elementum sapien, eget consequat erat venenatis non.',
      'Sed malesuada dui vitae nunc eleifend ut faucibus nulla consequat.',
      'In nec tellus porta velit tempus convallis.',
      'Aenean vestibulum ligula et enim blandit rutrum.',
      'Vestibulum molestie massa vel arcu tincidunt at elementum magna bibendum.',
      'Nam ac orci quis est faucibus tempor.',
      'Curabitur a diam a augue commodo ullamcorper sit amet et lacus.',
      'Vivamus at elit et lectus facilisis ultrices at ac diam.',
      'In condimentum magna at est laoreet condimentum mattis ipsum sagittis.',
      'Donec sed nisl quis eros tincidunt rutrum.',
      'Aenean eu dui eget mauris iaculis malesuada a vitae est.',
      'Aliquam quis diam eu tortor accumsan vehicula quis quis quam.',
      'In sed dui justo, eu adipiscing nunc.',
      'Vivamus vulputate risus consectetur purus ornare sed aliquam diam sodales.',
      'Praesent facilisis ullamcorper nisi, id volutpat turpis convallis id.',
      'Nam ac felis risus, quis tempus nisl.',
      'Pellentesque tempus tempus lorem, non molestie velit consequat non.',
      'Cras lacinia velit non ligula viverra ut mattis nibh euismod.',
      'Nulla at mi a enim rhoncus lobortis et non mi.',
      'Praesent placerat diam non nisi convallis sagittis ullamcorper mauris vulputate.',
      'Etiam suscipit nibh pellentesque risus semper in tincidunt ipsum ultricies.',
      'Vivamus a tellus id felis consectetur pretium.',
      'Duis semper sodales odio, ac hendrerit ipsum eleifend a.',
      'Aliquam ac erat vitae tellus ullamcorper imperdiet non nec dui.',
      'Aliquam gravida nisi in sem sodales eu pharetra ante tempus.',
      'Nulla et tellus at libero pellentesque mollis non et augue.',
      'Etiam volutpat orci quis urna condimentum vel rutrum mauris euismod.',
      'Donec sit amet orci tempor nunc tincidunt viverra.',
      'Nam at lacus quam, in facilisis erat.',
      'Aliquam sit amet massa ornare urna auctor gravida.',
      'Suspendisse luctus purus nec mauris mattis dictum id vel mi.',
      'Nam at leo quis orci sollicitudin bibendum ullamcorper vitae velit.',
      'Integer at erat vel enim lacinia tincidunt vel a dolor.',
      'Nullam mattis lobortis est, non mollis nisl aliquet in.',
      'Suspendisse eget ante ligula, id sodales sapien.',
      'Cras vel nibh nec quam blandit congue.',
      'Etiam quis arcu turpis, a pellentesque ante.',
      'Cras ac nulla sed nibh interdum accumsan.',
      'Duis eu massa odio, quis laoreet lectus.',
      'Ut vehicula tellus in metus auctor tincidunt.',
      'Sed sit amet sapien eget urna bibendum fringilla at quis arcu.',
      'Mauris vel dui ac ligula ullamcorper volutpat a a nisl.',
      'Praesent pulvinar rutrum odio, eget lobortis sapien semper at.',
      'Nunc a nunc urna, in tempor neque.',
      'Duis vulputate ultrices justo, ut placerat risus mattis quis.',
      'Nullam quis augue viverra libero facilisis commodo et ac nulla.',
      'Nunc eget purus in ante feugiat interdum ac ut eros.',
      'Etiam quis magna libero, sed sodales sem.',
      'Vivamus ac est ac ipsum dignissim dictum sit amet ut augue.',
      'Donec non mauris id tortor pulvinar egestas.',
      'Curabitur condimentum purus in felis fringilla egestas.',
      'Vestibulum hendrerit felis vitae ipsum dapibus semper.',
      'Vestibulum tempus metus eget sapien luctus ultricies.',
      'Sed ultricies justo commodo turpis tincidunt varius.',
      'Phasellus convallis justo nec libero porta a mattis urna volutpat.',
      'Sed id ipsum nibh, et malesuada lorem.',
      'Fusce a sapien id justo ultricies ultricies.',
      'Etiam sed ligula aliquet nulla congue viverra eu at urna.',
      'Suspendisse volutpat dui ut ligula dignissim rhoncus.',
      'Praesent volutpat justo eget orci pellentesque ut feugiat risus bibendum.',
      'Curabitur in lectus ipsum, quis lobortis mauris.',
      'Vestibulum a est quis leo semper congue.',
    ),
  );

  /***************************************************
       PUBLIC METHODS
  ***************************************************/
  
  public function __construct($subject, $options = array(), $content_data = array()) {
    parent::__construct($subject, $options, $content_data);
    
    // find all text subjects
      if (!empty($content_data['text'])) {
        foreach ($content_data['text'] as $category => $text) {
          // handle single-string cases
          if (!is_array($text)) {
            $text = array($text);
          }
          
          foreach($text as $line) {
            $this->content_db[$category][] = $line;
          }
        }
      }
    
    $this->data = isset($this->content_db[$this->subject]) ? $this->content_db[$this->subject] : $this->content_db['dc_random'];
    
    $this->size = sizeof($this->data);
  }
  
  public function generate($quantity, $options = array()) {
    $output = '';
    
    // validate the type option
    if (!empty($options['type']) && in_array($options['type'], $this->type_options)) {
      $type = $options['type'];
    }
    //default
    else {
      $type = 'word';
    }

    switch ($type) {
      case 'word':
        $output = $this->generate_word($quantity);
        
        break;
      
      case 'sentence':
        $output = $this->generate_sentence($quantity);
        break;
        
      case 'paragraph':
        $output = $this->generate_paragraph($quantity);
        break;
    }
    
    return $output;
  }
  
  /***************************************************
       PRIVATE METHODS
  ***************************************************/

  private function generate_word($quantity) {
    $data = array();
    
    static $word_bank = array();
    
    if (empty($word_bank)) {
      $limit = $this->size > 1 ? $this->size / 2 : 1;
      $limit = $limit > 9 ? 10 : $limit;      
      
      for ($i = 0; $i < $limit; $i++) {
        $word_bank[] = $this->data[array_rand($this->data)];
      }
      
      // clean up the word bank
      $word_bank = implode(' ', $word_bank);
      $word_bank = strtolower($word_bank);
      $word_bank = preg_replace('/[^a-z0-9\s]/', '', $word_bank); // clean special characters
      $word_bank = str_replace('  ', ' ', $word_bank); // clean duplicate spaces
      $word_bank = trim($word_bank);
      
      $word_bank = explode(' ', $word_bank);
    }

    $size = sizeof($word_bank);

    for ($i = 0; ($i < $quantity) && ($i < $size); $i++) {
      $data[] = $word_bank[array_rand($word_bank)];
    }

    $output = implode(' ', $data);
    
    // capitalize the first character
    $output{0} = strtoupper($output{0});
    
    return $output; 
  }
  
  private function generate_sentence($quantity) {
    $data = array();
    
    for ($i = 0; $i < $quantity; $i++) {
      $data[] = $this->data[array_rand($this->data)];
    }

    return implode(' ', $data);
  }
  
  private function generate_paragraph($quantity) {
    $data = array();
    
    for ($i = 0; $i < $quantity; $i++) {
      $data[] = $this->generate_sentence(mt_rand(1, 7));
    }
    
    return implode("\n\n", $data);
  }
}
