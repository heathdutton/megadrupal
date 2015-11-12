<?php

/**
 * @file
 * Destination classes.
 */

use \ChapterThree\AppleNewsAPI\Document\Margin;
use \ChapterThree\AppleNewsAPI\Document\Components;
use \ChapterThree\AppleNewsAPI\Document\Styles;
use \ChapterThree\AppleNewsAPI\Document\Layouts;
use \ChapterThree\AppleNewsAPI\Document\Behaviors;
use \ChapterThree\AppleNewsAPI\Document\Animations;
use \ChapterThree\AppleNewsAPI\Document\Markdown;

/**
 * Body component.
 */
class ApplenewsexampleDestinationBody extends ApplenewsDestinationBodyPhoto {

  /**
   * {@inheritdoc}
   */
  public function getSetting($key) {
    // Override parent settings.
    switch ($key) {

      case 'format':
        return 'markdown';

    }
    return parent::getSetting($key);
  }

  /**
   * {@inheritdoc}
   */
  public function getConfigForm(array $values = []) {
    $form = parent::getConfigForm($values);
    $form['settings']['format']['#type'] = 'value';
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function export() {

    $body_text_style = new Styles\ComponentTextStyle();
    $body_text_style->setTextAlignment('left')
      ->setFontName('AvenirNext-Regular')
      ->setFontSize(16)
      ->setLineHeight(26)
      ->setTextColor('#000');

    $dc_style = new Styles\DropCapStyle(5);
    $dc_style->setNumberOfCharacters(1)
      ->setFontName('AvenirNext-UltraLight')
      ->setTextColor('#222E33');
    $initial_body_text_style = clone($body_text_style);
    $initial_body_text_style->setDropCapStyle($dc_style);

    $body_inline_text_style = new Styles\InlineTextStyle(1, 19, $body_text_style);
    $body_layout = new Layouts\ComponentLayout();
    $body_layout->setMargin(new Margin(25, 25));

    $image_layout = new Layouts\ComponentLayout();

    switch ($this->export->getSetting('layout_alignment')) {

      case 'left':
        $body_layout->setColumnStart(0)->setColumnSpan(5);
        $image_layout->setColumnStart(3)->setColumnSpan(4);
        break;

      case 'center':
        $body_layout->setColumnStart(1)->setColumnSpan(7);
        $image_layout->setColumnStart(1)->setColumnSpan(7);
        break;

      case 'right':
        $body_layout->setColumnStart(2)->setColumnSpan(5);
        $image_layout->setColumnStart(0)->setColumnSpan(4);
        break;

    }

    $components = parent::export();
    for ($i = 0; $i < count($components); $i++) {
      $component = $components[$i];
      if ($component instanceof Components\Body) {
        /** @var \ChapterThree\AppleNewsAPI\Document\Components\Body $component */
        if ($component->getFormat() != 'markdown') {
          $component->addInlineTextStyles($body_inline_text_style);
        }
        $component->setLayout($body_layout);
        if ($i) {
          $component->setTextStyle($body_text_style);
        }
        else {
          $component->setTextStyle($initial_body_text_style);
        }
      }
      elseif ($component instanceof Components\ScalableImage) {
        /** @var \ChapterThree\AppleNewsAPI\Document\Components\ScalableImage $component */
        $component->setLayout($image_layout);
      }
    }

    return $components;
  }

}

/**
 * Image header component.
 */
class ApplenewsexampleDestinationHeaderImage extends ApplenewsDestinationComponent {

  /**
   * {@inheritdoc}
   */
  public function settings() {
    return [
      'file' => NULL,
    ] + parent::settings();
  }

  /**
   * {@inheritdoc}
   */
  public function validateSettings() {
    return $this->validateSettingsSources(['file']) &&
      parent::validateSettings();
  }

  /**
   * {@inheritdoc}
   */
  public function getConfigForm(array $values = []) {

    $form = parent::getConfigForm($values);

    $element = parent::getConfigFormSourceElement('file', TRUE, TRUE);
    $element['#title'] = t('File');
    $element['#required'] = TRUE;
    $form['settings']['file'] = $element;

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function export() {
    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('file'));
    if (empty($values[0][$key])) {
      return [];
    }

    $url = @$this->getSetting('file')['bundle']
      ? $this->export->addAsset($values[0][$key]) : $values[0][$key];
    $fill = new Styles\Fills\ImageFill($url);
    $fill->setFillMode('cover')->setVerticalAlignment('bottom');
    $style = new Styles\ComponentStyle();
    $style->setFill($fill);
    $header = new Components\Header();
    $header->setStyle($style);
    $layout = new Layouts\ComponentLayout();
    $layout->setIgnoreDocumentMargin()
      ->setMinimumHeight('42vh');
    $header->setLayout($layout);

    switch ($this->export->getSetting('layout_alignment')) {

      case 'left':
      case 'right':
        $layout->setColumnStart(0)->setColumnSpan(7);
        break;

      case 'center':
        $layout->setColumnStart(0)->setColumnSpan(9);
        break;

    }

    return [$header];
  }

}

/**
 * Gradient header container component.
 */
class ApplenewsexampleDestinationHeaderGradContainer extends ApplenewsDestinationComponent {

  /**
   * {@inheritdoc}
   */
  public function settings() {
    return [
      'title'        => NULL,
      'intro'        => NULL,
      'author'       => NULL,
      'photographer' => NULL,
      'image'        => NULL,
      'gradient'     => FALSE,
    ] + parent::settings();
  }

  /**
   * {@inheritdoc}
   */
  public function validateSettings() {
    return $this->validateSettingsSources(['title']) &&
      parent::validateSettings();
  }

  /**
   * {@inheritdoc}
   */
  public function getConfigForm(array $values = []) {

    $form = parent::getConfigForm($values);

    $element = parent::getConfigFormSourceElement('title');
    $element['#title'] = t('Title');
    $element['#required'] = TRUE;
    $form['settings']['title'] = $element;

    $element = parent::getConfigFormSourceElement('intro');
    $element['#title'] = t('Intro');
    $form['settings']['intro'] = $element;

    $element = parent::getConfigFormSourceElement('author');
    $element['#title'] = t('Author');
    $form['settings']['author'] = $element;

    $element = parent::getConfigFormSourceElement('photographer');
    $element['#title'] = t('Photographer');
    $form['settings']['photographer'] = $element;

    $element = parent::getConfigFormSourceElement('image', TRUE, TRUE);
    $element['#title'] = t('Background Image');
    $form['settings']['image'] = $element;

    $form['settings']['gradient'] = [
      '#type' => 'checkbox',
      '#title' => t('Gradient'),
      '#default_value' => $this->getSetting('gradient'),
      '#states' => [
        'visible' => [
          ':input[name="image[source]"]' => ['value' => ''],
        ],
      ]
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function export() {

    // Components container.
    $container = new Components\Container();
    $layout = new Layouts\ComponentLayout();
    $layout->setColumnStart(0)->setColumnSpan(7);
    $container->setLayout($layout);
    $container->setBehavior(new Behaviors\BackgroundParallax());

    // Divider.
    $divider = new Components\Divider();
    $layout = new Layouts\ComponentLayout();
    $layout->setColumnStart(0)->setColumnSpan(7)
      ->setMargin(new Margin(NULL, 15));
    $divider->setLayout($layout);
    $stroke = new Styles\StrokeStyle();
    $stroke->setColor('#E6E6E6')->setWidth(1);
    $divider->setStroke($stroke);
    $container->addComponent($divider);

    // Title.
    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('title'));
    if (empty($values[0][$key])) {
      return [];
    }
    $title = new Components\Title($values[0][$key]);
    $layout = new Layouts\ComponentLayout();
    $layout->setColumnStart(0)->setColumnSpan(7);
    $title->setLayout($layout);
    $text_style = new Styles\ComponentTextStyle();
    $text_style->setTextAlignment('center')
      ->setFontName('AvenirNext-UltraLight')
      ->setTextTransform('uppercase')
      ->setFontSize(64)
      ->setLineHeight(68)
      ->setTextColor('#FFF');
    $title->setTextStyle($text_style);
    $container->addComponent($title);

    // Divider.
    $divider = new Components\Divider();
    $layout = new Layouts\ComponentLayout();
    $layout->setColumnStart(0)->setColumnSpan(7)
      ->setMargin(new Margin(15, 15));
    $divider->setLayout($layout);
    $stroke = new Styles\StrokeStyle();
    $stroke->setColor('#E6E6E6')->setWidth(1);
    $divider->setStroke($stroke);
    $container->addComponent($divider);

    // Intro.
    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('intro'));
    if (!empty($values[0][$key])) {
      $intro = new Components\Intro($values[0][$key]);
      $layout = new Layouts\ComponentLayout();
      $layout->setColumnStart(0)->setColumnSpan(7)
        ->setMargin(new Margin(10, 5));
      $intro->setLayout($layout);
      $text_style = new Styles\ComponentTextStyle();
      $text_style->setTextAlignment('center')
        ->setFontName('AvenirNext-Regular')
        ->setFontSize(18)
        ->setLineHeight(26)
        ->setTextColor('#FFF');
      $intro->setTextStyle($text_style);
      $container->addComponent($intro);
    }

    /** @var EntityDrupalWrapper $wrapper */
    $wrapper = entity_metadata_wrapper('node', $this->export->source);
    $language = $wrapper->language->value();

    // Author.
    $text_style = new Styles\ComponentTextStyle();
    $text_style->setTextAlignment('center')
      ->setFontName('AvenirNextCondensed-DemiBold')
      ->setFontSize(13)
      ->setTextColor('#FFF');
    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('author'));
    if (!empty($values[0][$key])) {
      $author = t('By !author', ['!author' => $values[0][$key]],
        ['langcode' => $language]);
      $author = new Components\Author($author);
      $layout = new Layouts\ComponentLayout();
      $layout->setColumnStart(0)->setColumnSpan(7)
        ->setMargin(new Margin(10, 5));
      $author->setLayout($layout);
      $author->setTextStyle($text_style);
      $container->addComponent($author);
    }
    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('photographer'));
    if (!empty($values[0][$key])) {
      $photographer = t('Photos: !photographer',
        ['!photographer' => $values[0][$key]],
        ['langcode' => $language]);
      $photographer = new Components\Photographer($photographer);
      $layout = new Layouts\ComponentLayout();
      $layout->setColumnStart(0)->setColumnSpan(7)
        ->setMargin(new Margin(NULL, 15));
      $photographer->setLayout($layout);
      $photographer->setTextStyle($text_style);
      $container->addComponent($photographer);
    }

    // Parent container.
    $header = new Components\Header();
    $layout = new Layouts\ComponentLayout();
    $layout->setColumnStart(0)->setColumnSpan(7)
      ->setIgnoreDocumentMargin()
      ->setMinimumHeight('40vh')
      ->setMargin(new Margin(NULL, 25));
    $header->setLayout($layout);
    $header->addComponent($container);

    // Image or gradient.
    $fill = FALSE;
    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('image'));
    if (!empty($values[0][$key])) {
      $url = @$this->getSetting('items')['bundle']
        ? $this->export->addAsset($values[0][$key]) : $values[0][$key];
      $fill = new Styles\Fills\ImageFill($url);
      $fill->setFillMode('cover')->setVerticalAlignment('bottom');
    }
    else {
      if ($this->getSetting('gradient')) {
        $fill = new Styles\Fills\Gradients\LinearGradientFill([
          new Styles\Fills\Gradients\ColorStop('#263236'),
          new Styles\Fills\Gradients\ColorStop('#3B4A4E'),
          new Styles\Fills\Gradients\ColorStop('#6D8A95'),
          new Styles\Fills\Gradients\ColorStop('#FFFFFF'),
        ]);
        $fill->setAngle(180);
      }
    }
    if ($fill) {
      $style = new Styles\ComponentStyle();
      $style->setFill($fill);
    }
    else {
      $style = new Styles\ComponentStyle();
      $style->setBackgroundColor('#3B4A4E');
    }
    $header->setStyle($style);

    return [$header];
  }

}

/**
 * Animated pullquote component.
 */
class ApplenewsexampleDestinationPullQuote extends ApplenewsDestinationComponent {

  /**
   * {@inheritdoc}
   */
  public function settings() {
    return [
      'image'  => NULL,
      'quote'  => NULL,
      'format' => 'markdown',
    ] + parent::settings();
  }

  /**
   * {@inheritdoc}
   */
  public function validateSettings() {
    return $this->validateSettingsSources(['quote']) &&
      parent::validateSettings();
  }

  /**
   * {@inheritdoc}
   */
  public function getConfigForm(array $values = []) {

    $form = parent::getConfigForm($values);

    $element = parent::getConfigFormSourceElement('image', TRUE, TRUE);
    $element['#title'] = t('Image');
    $form['settings']['image'] = $element;

    $element = parent::getConfigFormSourceElement('quote');
    $element['#title'] = t('Quote');
    $element['#required'] = TRUE;
    $form['settings']['quote'] = $element;

    $form['settings']['format'] = [
      '#title'         => t('Quote Format'),
      '#type'          => 'select',
      '#options'       => [
        ''         => t('None'),
        'markdown' => t('Markdown'),
      ],
      '#default_value' => $this->getSetting('format'),
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function export() {

    // Pullquote.
    list($key, $quote) = $this->getSourceValues($this->export->source,
      $this->getSetting('quote'));
    if (empty($quote[0][$key])) {
      return [];
    }
    $quote = $quote[0][$key];
    if ($this->getSetting('format') == 'markdown') {
      $markdown = new Markdown();
      $quote = $markdown->convert($quote);
      if (empty($quote)) {
        return [];
      }
    }
    $quote = new Components\Pullquote($quote);
    if ($this->getSetting('format')) {
      $quote->setFormat($this->getSetting('format'));
    }

    // Background.
    list($key, $file) = $this->getSourceValues($this->export->source,
      $this->getSetting('image'));

    // Pullquote without background.
    if (empty($file[0][$key])) {

      // Pullquote.
      $layout = new Layouts\ComponentLayout();
      $layout->setIgnoreDocumentMargin()
        ->setMinimumHeight('42vh');
      $quote->setLayout($layout);
      $text_style = new Styles\ComponentTextStyle();
      $text_style->setTextTransform('uppercase')
        ->setFontName('AvenirNext-Bold')
        ->setFontSize(48)
        ->setLineHeight(60);
      $quote->setTextStyle($text_style);

      switch ($this->export->getSetting('layout_alignment')) {

        case 'left':
          // Quote.
          /** @var \ChapterThree\AppleNewsAPI\Document\Layouts\ComponentLayout $layout */
          $layout = $quote->getLayout();
          $layout->setColumnStart(3)->setColumnSpan(4);
          /** @var \ChapterThree\AppleNewsAPI\Document\Styles\ComponentTextStyle $text_style */
          $text_style = $quote->getTextStyle();
          $text_style->setTextAlignment('left');
          break;

        case 'center':
          // Quote.
          /** @var \ChapterThree\AppleNewsAPI\Document\Layouts\ComponentLayout $layout */
          $layout = $quote->getLayout();
          $layout->setColumnStart(1)->setColumnSpan(7);
          /** @var \ChapterThree\AppleNewsAPI\Document\Styles\ComponentTextStyle $text_style */
          $text_style = $quote->getTextStyle();
          $text_style->setTextAlignment('center');
          break;

        case 'right':
          // Quote.
          /** @var \ChapterThree\AppleNewsAPI\Document\Layouts\ComponentLayout $layout */
          $layout = $quote->getLayout();
          $layout->setColumnStart(0)->setColumnSpan(4);
          /** @var \ChapterThree\AppleNewsAPI\Document\Styles\ComponentTextStyle $text_style */
          $text_style = $quote->getTextStyle();
          $text_style->setTextAlignment('left');
          break;

      }

      return [$quote];

    }

    // Pullquote with background.
    // Parent section.
    $section = new Components\Section();
    $section->setScene(new Animations\Scenes\ParallaxScaleHeader());
    $layout = new Layouts\ComponentLayout();
    $layout->setIgnoreDocumentMargin()
      ->setMinimumHeight('42vh');
    $section->setLayout($layout);

    // Header.
    $header = new Components\Header();
    $url = @$this->getSetting('image')['bundle']
      ? $this->export->addAsset($file[0][$key]) : $file[0][$key];
    if ($url) {
      $fill = new Styles\Fills\ImageFill($url);
      $fill->setFillMode('cover')->setVerticalAlignment('top');
      $style = new Styles\ComponentStyle();
      $style->setFill($fill);
      $header->setStyle($style);
    }
    $layout = new Layouts\ComponentLayout();
    $layout->setIgnoreDocumentMargin()
      ->setMinimumHeight('75vw')
      ->setMargin(new Margin(25, 25));
    $header->setLayout($layout);

    // Pullquote.
    $layout = new Layouts\ComponentLayout();
    $layout->setColumnStart(0)->setColumnSpan(7)->setMargin(new Margin(70));
    $quote->setLayout($layout);
    $quote->setBehavior(new Behaviors\BackgroundParallax());
    $text_style = new Styles\ComponentTextStyle();
    $text_style->setTextTransform('uppercase')
      ->setFontName('AvenirNext-Bold')
      ->setFontSize(48)
      ->setLineHeight(20)
      ->setTextColor('#FFF');
    $quote->setTextStyle($text_style);

    $header->addComponent($quote);
    $section->addComponent($header);

    switch ($this->export->getSetting('layout_alignment')) {

      case 'left':
        // Quote.
        /** @var \ChapterThree\AppleNewsAPI\Document\Layouts\ComponentLayout $layout */
        $layout = $quote->getLayout();
        $layout->setColumnStart(1);
        /** @var \ChapterThree\AppleNewsAPI\Document\Styles\ComponentTextStyle $text_style */
        $text_style = $quote->getTextStyle();
        $text_style->setTextAlignment('left');
        // Section.
        /** @var \ChapterThree\AppleNewsAPI\Document\Layouts\ComponentLayout $layout */
        $layout = $section->getLayout();
        $layout->setColumnStart(3)->setColumnSpan(4);
        break;

      case 'center':
        // Quote.
        /** @var \ChapterThree\AppleNewsAPI\Document\Layouts\ComponentLayout $layout */
        $layout = $quote->getLayout();
        $layout->setColumnStart(1)->setColumnSpan(5);
        /** @var \ChapterThree\AppleNewsAPI\Document\Styles\ComponentTextStyle $text_style */
        $text_style = $quote->getTextStyle();
        $text_style->setTextAlignment('center');
        // Section.
        /** @var \ChapterThree\AppleNewsAPI\Document\Layouts\ComponentLayout $layout */
        $layout = $section->getLayout();
        $layout->setColumnStart(1)->setColumnSpan(7);
        break;

      case 'right':
        // Quote.
        /** @var \ChapterThree\AppleNewsAPI\Document\Layouts\ComponentLayout $layout */
        $layout = $quote->getLayout();
        $layout->setColumnSpan(6);
        /** @var \ChapterThree\AppleNewsAPI\Document\Styles\ComponentTextStyle $text_style */
        $text_style = $quote->getTextStyle();
        $text_style->setTextAlignment('left');
        // Section.
        /** @var \ChapterThree\AppleNewsAPI\Document\Layouts\ComponentLayout $layout */
        $layout = $section->getLayout();
        $layout->setColumnStart(0)->setColumnSpan(4);
        break;

    }

    return [$section];
  }

}

/**
 * Mosaic or Gallery component.
 */
class ApplenewsexampleDestinationImages extends ApplenewsDestinationImages {

  /**
   * {@inheritdoc}
   */
  public function export() {
    $components = parent::export();
    if (empty($components)) {
      return [];
    }

    /** @var \ChapterThree\AppleNewsAPI\Document\Components\Mosaic|\ChapterThree\AppleNewsAPI\Document\Components\Gallery $component */
    $component = $components[0];
    $animation = new Animations\ComponentAnimations\MoveInAnimation();
    $animation->setPreferredStartingPosition('right');
    $component->setAnimation($animation);
    $layout = new Layouts\ComponentLayout();
    $layout
      ->setIgnoreDocumentMargin('left')
      ->setMargin(new Margin(25, 25));
    $component->setLayout($layout);

    switch ($this->export->getSetting('layout_alignment')) {

      case 'left':
      case 'right':
        $layout->setColumnStart(0)->setColumnSpan(7);
        break;

      case 'center':
        $layout->setColumnStart(0)->setColumnSpan(9);
        break;

    }

    return [$component];
  }

}

/**
 * Banner Ad.
 */
class ApplenewsexampleDestinationAd extends ApplenewsDestinationAd {

  /**
   * {@inheritdoc}
   */
  public function getSetting($key) {
    // Override parent settings.
    switch ($key) {

      case 'type':
        return 'banner';

      case 'banner_type':
        return 'standard';

    }
    return parent::getSetting($key);
  }

  /**
   * {@inheritdoc}
   */
  public function getConfigForm(array $values = []) {
    $form = parent::getConfigForm($values);
    $form['settings']['type']['#type'] = 'value';
    $form['settings']['banner_type']['#type'] = 'value';
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function export() {
    $components = parent::export();
    if (empty($components)) {
      return [];
    }
    /** @var \ChapterThree\AppleNewsAPI\Document\Components\Advertisements\BannerAdvertisement $component */
    $component = $components[0];

    $layout = new Layouts\ComponentLayout();
    $component->setLayout($layout);

    switch ($this->export->getSetting('layout_alignment')) {

      case 'left':
      case 'right':
        $layout->setColumnStart(0)->setColumnSpan(7);
        break;

      case 'center':
        $layout->setColumnStart(1)->setColumnSpan(7);
        break;

    }

    return $components;
  }

}
