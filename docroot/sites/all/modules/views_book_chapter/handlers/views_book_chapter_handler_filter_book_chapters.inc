<?php
/**
 * @file
 * Definition of views_book_chapter_handler_filter_book_chapters.
 */

/**
 * Books chapters filter.
 *
 * @ingroup views_filter_handlers
 */
class views_book_chapter_handler_filter_book_chapters extends views_handler_filter_many_to_one {

  /**
   * This force the "settings" in the filter.
   */
  function has_extra_options() { return TRUE; }

  function get_value_options() { }

  function option_definition() {
    $options = parent::option_definition();

    $options['book_show_childs'] = array('default' => FALSE, 'bool' => FALSE);
    $options['book'] = array('default' => 0);
    $options['book_add_childs'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  /**
   * First step in the settings filter.
   */
  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);

    $form['book_show_childs'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Show chapters childs'),
      '#description'   => t('Useful if you need select an especified chapters child. Normal only show the first child of the book.'),
      '#default_value' => $this->options['book_show_childs'],
    );

    $books = _views_book_chapter_get_books();
    if (count($books) < 8) {

      $form['book'] = array(
        '#type'          => 'select',
        '#title'         => t('Book'),
        '#options'       => $books,
        '#default_value' => $this->options['book'],
      );
    }
    else {
      $form['book'] = array(
        '#type'              => 'textfield',
        '#autocomplete_path' => 'views-book-chapter/autocomplete/',
        '#default_value' => $this->options['book'],
        '#attributes'        => array(
          'class'       => array(),
          'placeholder' => t('Search your book'),
        ),
      );
    }
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['book_add_childs'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Add childs chapter.'),
      '#description'   => t('Add recursive childs chapters.'),
      '#default_value' => $this->options['book_add_childs'],
    );
  }

  /**
   * Final chapter selection.
   */
  function value_form(&$form, &$form_state) {

    $options = array();
    if (!empty($this->options['book'])) {
      $options = views_book_chapter_get_book_tree($this->options['book'], $this->options['book_show_childs']);
    }

    $default_value = (array) $this->value;

    $form['value'] = array(
      '#type'          => 'select',
      '#title'         => t('Chapters list.'),
      '#multiple'      => TRUE,
      '#options'       => $options,
      '#size'          => min(9, count($options)),
      '#default_value' => $default_value,
    );

  }

  function admin_summary() {

    $this->value_options = array();

    if ($this->value) {
      $this->value = array_filter($this->value);

      $select = db_select('menu_links', 'ml');
      $select
        ->leftJoin('book', 'b', 'ml.mlid = b.mlid');

      $result = $select
        ->fields('ml', array('link_title', 'mlid'))
        ->condition('ml.mlid', array($this->value), 'IN');

      foreach ($result->execute() as $book_chapter) {
        $this->value_options[$book_chapter->mlid] = $book_chapter->link_title;
      }

    }

    return parent::admin_summary();
  }

  function query() {
    $info = $this->operators();

    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}();
    }

    if (empty($this->options['book_add_childs']) && empty($this->value)) {
      return;
    }
    else {
      $this->ensure_my_table();

      $select = db_select('menu_links', 'ml');
      $result = $select
        ->fields('ml', array('mlid', 'p1', 'p2', 'p3', 'p4', 'p4', 'p5', 'p6', 'p7', 'p8', 'p9'))
        ->condition('ml.mlid', $this->value, 'IN')
        ->execute()
        ->fetchAllAssoc('mlid', PDO::FETCH_ASSOC);

      if ($result) {
        foreach ($result as $key => $value) {
          $value_fliped = array_flip($value);

          $condition = db_or()
            ->condition("{$this->table_alias}.{$value_fliped[$key]}" , $key);

          $this->query->add_where($this->options['group'], $condition);
        }
      }
    }
  }
}
