<?php

/**
 * @file
 * Export classes.
 */

use ChapterThree\AppleNewsAPI\Document;
use \ChapterThree\AppleNewsAPI\Document\Anchor;
use \ChapterThree\AppleNewsAPI\Document\Components;

/**
 * A configurable node export.
 */
class ApplenewsexampleExportNode extends ApplenewsExportNode {

  /**
   * {@inheritdoc}
   */
  public function settings() {
    return [
      'layout_alignment' => 'left',
    ] + parent::settings();
  }

  /**
   * {@inheritdoc}
   */
  public function getConfigForm(array $values = []) {
    $form = parent::getConfigForm($values);

    // Hide parent layout elements.
    $tab = &$form['additional_settings']['layouts_tab'];
    $tab['layout']['#access'] = FALSE;
    $tab['component_layouts']['#access'] = FALSE;

    $tab['layout_alignment'] = [
      '#type'          => 'select',
      '#title'         => t('Alignment'),
      '#required'      => TRUE,
      '#default_value' => $this->getSetting('layout_alignment'),
      '#options'       => [
        'left'   => t('Left'),
        'center' => t('Center'),
        'right'  => t('Right'),
      ],
    ];

    return $form;
  }

  /**
   * Set any relations between components.
   */
  protected function associateComponents() {

    $components = $this->document->getComponents();
    for ($i = 0; $i < count($components); $i++) {

      /** @var \ChapterThree\AppleNewsAPI\Document\Components\Component $comp */
      $comp = $components[$i];

      // Associate with previous component.
      if ($i) {

        $prev = $components[$i - 1];

        // Previous is a body component.
        /** @var \ChapterThree\AppleNewsAPI\Document\Components\Body $prev */
        if ($prev instanceof Components\Body) {

          // Anchor to previous component if not anchor-ed already.
          if (!$comp->getAnchor()) {
            // One of:
            $anchor = $comp instanceof Components\Image ||
              $comp instanceof Components\ScalableImage ||
              $comp instanceof Components\Pullquote;
            $range_start = 0;
            // Or contains pullquote.
            if (!$anchor && $comp instanceof Components\ComponentNested) {
              $class_pullquote = '\ChapterThree\AppleNews\Document\Components\Pullquote';
              /** @var \ChapterThree\AppleNewsAPI\Document\Components\ComponentNested $comp */
              if ($comp->hasComponentType($class_pullquote)) {
                $anchor = TRUE;
                $range_start = floor(strlen($prev->getText()) / 2);
              }
            }
            if ($anchor) {
              if (!$id = $prev->getIdentifier()) {
                $id = $this->generateIdentifier();
                $prev->setIdentifier($id);
              }
              $anchor = new Anchor('bottom');
              $anchor->setOriginAnchorPosition('bottom')
                ->setTargetComponentIdentifier($id)
                ->setRangeStart($range_start)
                ->setRangeLength(1);
              $comp->setAnchor($anchor);
            }
          }
        }
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function exportDocument() {
    parent::exportDocument();

    $layout = $this->document->getLayout();
    switch ($this->getSetting('layout_alignment')) {

      case 'left':
      case 'right':
        $layout->setWidth(1024)->setColumns(7);
        break;

      case 'center':
        $layout->setWidth(768)->setColumns(9);
        break;

    }
    $layout->setMargin(75)->setGutter(20);
    $this->associateComponents();
    return $this->document;
  }

}
