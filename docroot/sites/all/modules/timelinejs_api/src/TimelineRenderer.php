<?php

/**
 * @file
 * Contains \Drupal\timelinejs_api\TimelineRenderer.
 */

namespace Drupal\timelinejs_api;

/**
 * Handles timeline JS rendering of data.
 */
class TimelineRenderer {

  /**
   * A unique ID for the timelime element.
   *
   * @var string
   */
  protected $id;

  /**
   * The timeline data to render.
   *
   * @var \Drupal\timelinejs_api\TimelineData
   */
  protected $data;

  /**
   * TimelineRenderer constructor.
   *
   * @param string $id
   *   The HTML id for the timelime element.
   * @param \Drupal\timelinejs_api\TimelineData|string $data
   *   A timeline data instance to pass data from or a path that will be AJAX
   *   loaded by timelineJS.
   */
  public function __construct($id, $data) {
    // Assign a unique ID now.
    $this->id = drupal_html_id($id);
    $this->data = $data;
  }

  /**
   * Builds a render array for the timeline.
   *
   * @return array
   */
  public function render() {
    return [
      'container' => [
        '#type' => 'container',
        '#attributes' => [
          'id' => $this->id,
          'class' => ['timelinejs-api-timeline'],
        ],
      ],
      '#attached' => [
        'libraries_load' => [['TimelineJS3']],
        'library' => [
          ['timelinejs_api', 'timeline'],
        ],
        'js' => [
          [
            'type' => 'setting',
            'data' => [
              'timelinejs_api' => $this->prepareSettings(),
            ],
          ],
        ],
      ],
    ];
  }

  /**
   * Prepares Drupal JS settings.
   *
   * @return array
   */
  protected function prepareSettings() {
    // If the data is a TimelineData object, Add the data in the JS settings.
    if ($this->data instanceof TimelineData) {
      $data = $this->data->toArray();

      // If there are no events, return no data. This will prevent the timeline
      // JS trying to create a timeline for something with no events. Only do
      // this when preparing data, as it will not work when using any AJAX path
      // loading.
      // @see timelinejs-api.js for the workaround for empty events when data is
      // AJAX loaded.
      if (empty($data['events'])) {
        $data =  [];
      }
    }
    // Otherwise, just add the file path, as-is.
    else {
      $data = $this->data;
    }

    return [
      // Key the settings/data by the element ID.
      $this->id => [
        'id' => $this->id,
        'data' => $data,
      ],
    ];
  }

}
