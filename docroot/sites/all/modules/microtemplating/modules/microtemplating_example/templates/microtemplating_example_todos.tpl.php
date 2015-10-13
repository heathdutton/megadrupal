<?php
/**
 * @file
 * Todo application : main template.
 */

?>
<section id="todoapp__container">
  <section id="todoapp">
    <header id="todoapp__header">
      <h1><?php print t('todos'); ?></h1>
      <input id="todoapp__new-todo" placeholder="<?php print t('What needs to be done?'); ?>" autofocus>
    </header>
    <section id="todoapp__main">
      <input id="todoapp__toggle-all" type="checkbox">
      <label for="toggle-all"><?php print t('Mark all as complete'); ?></label>
      <ul id="todoapp__todo-list"></ul>
    </section>
    <footer id="todoapp__footer"></footer>
  </section>
  <footer id="todoapp__info">
    <p><?php print t('Double-click to edit a todo'); ?></p>
    <p><?php print t('Created by !author', array('!author' => '<a href="http://sindresorhus.com">Sindre Sorhus</a>')); ?></p>
    <p><?php print t('Part of !link', array('!link' => '<a href="http://todomvc.com">TodoMVC</a>')); ?></p>
    <p><?php print t('Adapted to Drupal by !author', array('!author' => '<a href="https://www.drupal.org/u/tocab">tocab</a>')); ?></p>
  </footer>
</section>
