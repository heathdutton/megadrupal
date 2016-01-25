<?php
/**
 * @file
 * Todo application : todo list JavaScript template.
 */

?>
<% for (var i in todos) { %>
  <li data-id="<%= todos[i].id %>" <% if (todos[i].completed) { %>class="completed"<% } %>>
    <div class="view">
      <input class="toggle" type="checkbox" <% if (todos[i].completed) { %>checked<% } %>>
      <label><%= todos[i].title %></label>
      <button class="destroy todoapp__button"></button>
    </div>
    <input class="edit" value="<%= todos[i].title %>">
  </li>
<% } %>
