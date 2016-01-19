<?php
/**
 * @file
 * Todo application : footer JavaScript template.
 */

?>
<span id="todoapp__todo-count">
  <strong><%= activeTodoCount %></strong>
  <%= Drupal.formatPlural(activeTodoCount, 'item left', 'items left') %>
</span>
<ul id="todoapp__filters">
  <li>
    <a href="#/all" class="todoapp__filter <% if (filter == 'all') { %>selected<% } %>">
      <%= Drupal.t('All') %>
    </a>
  </li>
  <li>
    <a href="#/active" class="todoapp__filter <% if (filter == 'active') { %>selected<% } %>">
      <%= Drupal.t('Active') %>
    </a>
  </li>
  <li>
    <a href="#/completed" class="todoapp__filter <% if (filter == 'completed') { %>selected<% } %>">
      <%= Drupal.t('Completed') %>
    </a>
  </li>
</ul>
<% if (completedTodos) { %>
  <button id="todoapp__clear-completed" class="todoapp__button">
    <%= Drupal.t('Clear completed (@count)', {'@count': completedTodos}) %>
  </button>
<% } %>
