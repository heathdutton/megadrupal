/**
 * @file
 * Todo application.
 *
 * @see https://github.com/tastejs/todomvc/tree/gh-pages/examples/jquery
 */

(function($, tmpl) {
  'use strict';

  var ENTER_KEY  = 13;
  var ESCAPE_KEY = 27;

  var util = {
    uuid: function () {
      /*jshint bitwise:false */
      var i, random;
      var uuid = '';

      for (i = 0; i < 32; i++) {
        random = Math.random() * 16 | 0;
        if (i === 8 || i === 12 || i === 16 || i === 20) {
          uuid += '-';
        }
        uuid += (i === 12 ? 4 : (i === 16 ? (random & 3 | 8) : random)).toString(16);
      }

      return uuid;
    },

    store: function (namespace, data) {
      if (arguments.length > 1) {
        return localStorage.setItem(namespace, JSON.stringify(data));
      } else {
        var store = localStorage.getItem(namespace);
        return (store && JSON.parse(store)) || [];
      }
    }
  };

  var TodoApp = function(element) {
    this.todos = util.store('todos');
    this.$todoApp = $(element);
    this.cacheElements();
    this.bindEvents();
    this.route();
  };

  TodoApp.prototype = {
    cacheElements: function () {
      this.todoTemplate = tmpl('js__microtemplating_example__todo');
      this.footerTemplate = tmpl('js__microtemplating_example__footer');
      this.$header = this.$todoApp.find('#todoapp__header');
      this.$main = this.$todoApp.find('#todoapp__main');
      this.$footer = this.$todoApp.find('#todoapp__footer');
      this.$newTodo = this.$header.find('#todoapp__new-todo');
      this.$toggleAll = this.$main.find('#todoapp__toggle-all');
      this.$todoList = this.$main.find('#todoapp__todo-list');
      this.$count = this.$footer.find('#todoapp__todo-count');
      this.$clearBtn = this.$footer.find('#todoapp__clear-completed');
    },

    bindEvents: function () {
      var list = this.$todoList;
      this.$newTodo.bind('keyup', this.create.bind(this));
      this.$toggleAll.bind('change', this.toggleAll.bind(this));
      this.$footer.delegate('#todoapp__clear-completed', 'click', this.destroyCompleted.bind(this));
      list.delegate('.toggle', 'change', this.toggle.bind(this));
      list.delegate('label', 'dblclick', this.edit.bind(this));
      list.delegate('.edit', 'keyup', this.editKeyup.bind(this));
      list.delegate('.edit', 'focusout', this.update.bind(this));
      list.delegate('.destroy', 'click', this.destroy.bind(this));
      window.onhashchange = this.route.bind(this);
    },

    route: function () {
      if (window.location.hash.length > 1) {
        this.filter = window.location.hash.substr(2);
      } else {
        this.filter = 'all';
      }
      this.render();
    },

    render: function () {
      var todos = this.getFilteredTodos();
      this.$todoList.html(this.todoTemplate({todos: todos}));
      this.$main.toggle(todos.length > 0);
      this.$toggleAll.attr('checked', this.getActiveTodos().length === 0);
      this.renderFooter();
      this.$newTodo.focus();
      util.store('todos', this.todos);
    },

    renderFooter: function () {
      var todoCount = this.todos.length;
      var activeTodoCount = this.getActiveTodos().length;
      var template = this.footerTemplate({
        activeTodoCount: activeTodoCount,
        completedTodos: todoCount - activeTodoCount,
        filter: this.filter
      });

      this.$footer.toggle(todoCount > 0).html(template);
    },

    toggleAll: function (e) {
      var isChecked = $(e.target).attr('checked');

      this.todos.forEach(function (todo) {
        todo.completed = isChecked;
      });

      this.render();
    },

    getActiveTodos: function () {
      return this.todos.filter(function (todo) {
        return !todo.completed;
      });
    },

    getCompletedTodos: function () {
      return this.todos.filter(function (todo) {
        return todo.completed;
      });
    },

    getFilteredTodos: function () {
      if (this.filter === 'active') {
        return this.getActiveTodos();
      }

      if (this.filter === 'completed') {
        return this.getCompletedTodos();
      }

      return this.todos;
    },

    destroyCompleted: function () {
      this.todos = this.getActiveTodos();
      this.filter = 'all';
      this.render();
    },

    // Accepts an element from inside the `.item` div and
    // returns the corresponding index in the `todos` array.
    indexFromEl: function (el) {
      var id = $(el).closest('li').data('id');
      var todos = this.todos;
      var i = todos.length;

      while (i--) {
        if (todos[i].id === id) {
          return i;
        }
      }
    },

    create: function (e) {
      var $input = $(e.target);
      var val = $input.val().trim();

      if (e.which !== ENTER_KEY || !val) {
        return;
      }

      this.todos.push({
        id: util.uuid(),
        title: val,
        completed: false
      });

      $input.val('');

      this.render();
    },

    toggle: function (e) {
      var i = this.indexFromEl(e.target);
      this.todos[i].completed = !this.todos[i].completed;
      this.render();
    },

    edit: function (e) {
      var $input = $(e.target).closest('li').addClass('editing').find('.edit');
      $input.val($input.val()).focus();
    },

    editKeyup: function (e) {
      if (e.which === ENTER_KEY) {
        e.target.blur();
      }

      if (e.which === ESCAPE_KEY) {
        $(e.target).data('abort', true).blur();
      }
    },

    update: function (e) {
      var el = e.target;
      var $el = $(el);
      var val = $el.val().trim();

      if ($el.data('abort')) {
        $el.data('abort', false);
        this.render();
        return;
      }

      var i = this.indexFromEl(el);

      if (val) {
        this.todos[i].title = val;
      } else {
        this.todos.splice(i, 1);
      }

      this.render();
    },

    destroy: function (e) {
      this.todos.splice(this.indexFromEl(e.target), 1);
      this.render();
    }
  };

  window.TodoApp = TodoApp;
})(jQuery, tmpl);
