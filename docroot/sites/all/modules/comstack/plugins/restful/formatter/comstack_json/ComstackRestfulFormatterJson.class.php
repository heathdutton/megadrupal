<?php

/**
 * @file
 * Contains ComstackRestfulFormatterJson.
 */

class ComstackRestfulFormatterJson extends \RestfulFormatterJson {
  /**
   * {@inheritdoc}
   */
  public function prepare(array $data) {
    // If we're returning an error then set the content type to
    // 'application/problem+json; charset=utf-8'.
    if (!empty($data['status']) && floor($data['status'] / 100) != 2) {
      $this->contentType = 'application/problem+json; charset=utf-8';
      return $data;
    }

    $output = array(
      'data' => $data,
    );

    if (!empty($this->handler)) {
      if (method_exists($this->handler, 'getTotalCount') && method_exists($this->handler, 'isListRequest') && $this->handler->isListRequest()) {
        // Get the total number of items for the current request without pagination.
        $output['paging'] = array();
        $output['paging']['range'] = intval($this->handler->getRange());
        $output['paging']['total'] = intval($this->handler->getTotalCount());
      }

      if (method_exists($this->handler, 'additionalHateoas')) {
        $output = array_merge($output, $this->handler->additionalHateoas());
      }

      // Add HATEOAS to the output.
      $this->addHateoas($output);
    }

    return $output;
  }

  /**
   * Add HATEOAS links to list of item.
   *
   * @param $data
   *   The data array after initial massaging.
   */
  protected function addHateoas(array &$data) {
    if (!$this->handler) {
      return;
    }
    $request = $this->handler->getRequest();

    // Get self link.
    if (!isset($data['self'])) {
      $data['self'] = array(
        'title' => 'Self',
        'href' => $this->handler->versionedUrl($this->handler->getPath()),
      );
    }

    // Bail out now if not adding any paging information.
    if (!isset($data['paging'])) {
      return;
    }

    $page = !empty($request['page']) ? intval($request['page']) : 1;
    // If not cursor paging, include the current page.
    if (!isset($data['paging']['cursors'])) {
      $data['paging']['current_page'] = $page;
    }

    if (!isset($data['paging']['previous']) && $page > 1) {
      $request['page'] = $page - 1;
      $data['paging']['previous'] = array(
        'title' => 'Previous',
        'href' => $this->handler->getUrl($request),
      );
    }

    if (!isset($data['paging']['next'])) {
      // We know that there are more pages if the total count is bigger than the
      // number of items of the current request plus the number of items in
      // previous pages.
      $items_per_page = $this->handler->getRange();
      $previous_items = ($page - 1) * $items_per_page;
      if (isset($data['paging']['total']) && $data['paging']['total'] > count($data['paging']['total']) + $previous_items) {
        $request['page'] = $page + 1;
        $data['paging']['next'] = array(
          'title' => 'Next',
          'href' => $this->handler->getUrl($request),
        );
      }
    }
  }
}
