/*
  ======== A Handy Little QUnit Reference ========
  http://api.qunitjs.com/

  Test methods:
    module(name, {[setup][ ,teardown]})
    test(name, callback)
    expect(numberOfAssertions)
    stop(increment)
    start(decrement)
  Test assertions:
    ok(value, [message])
    equal(actual, expected, [message])
    notEqual(actual, expected, [message])
    deepEqual(actual, expected, [message])
    notDeepEqual(actual, expected, [message])
    strictEqual(actual, expected, [message])
    notStrictEqual(actual, expected, [message])
    throws(block, [expected], [message])
*/

/* global Drupal */

(function($) {

module('Drupal.behave', {
  // This will run before each test in this module.
  setup: function() {
    // var $fixture = $('#qunit-fixture');
  }
});

test('requires name argument', function (assert) {
  assert.throws(function () { Drupal.behave(); }, /name required/, 'throws name is required');
});

test('initializes with expected state', function (assert) {
  var behave = Drupal.behave('foo'),
      behavior = behave.behavior();

  assert.strictEqual(typeof behave, 'object', 'returns object');
  ['behave', 'behavior', 'attach', 'detach', 'ready', 'extend']
    .forEach(function (api) {
      assert.strictEqual(typeof behave[api], 'function', 'has ' + api + ' API function');
    });

  ['behave', 'behavior'].forEach(function (api) {
    assert.strictEqual(typeof behave[api](), 'object', '.' + api + ' returns object');
  });

  assert.strictEqual(behavior.name, 'foo', 'has name property on behavior');
  assert.strictEqual(typeof behavior._behave, 'object', 'has _behave on behavior');
  assert.strictEqual(typeof behavior._behave.options, 'object', 'has _behave.options on behavior');
  assert.strictEqual(behavior._behave.options.only, document, 'options.only defaults to document');
});

test('API is chainable', function (assert) {
  var behave = Drupal.behave('bar');

  ['attach', 'detach', 'ready', 'extend']
    .forEach(function (api) {
      assert.strictEqual(behave[api](), behave.behave().api, '.' + api + ' returns chainable API');
    });
});

test('attach, ready, detach called with correct arguments', function (assert) {
  var behave = Drupal.behave('baz'),
      isCalled = {};

  behave
    .attach(function () {
      isCalled.attach = {arguments: arguments, this: this};
    })
    .ready(function () {
      isCalled.ready = {arguments: arguments, this: this};
    })
    .detach(function () {
      isCalled.detach = {arguments: arguments, this: this};
    });

  // Mock call Drupal.attachBehaviors
  Drupal.behaviors.baz.attach(document, {foo: true, bar: false});

  assert.ok(isCalled.attach, 'attach is called');
  assert.ok(isCalled.ready, 'ready is called');

  assert.strictEqual(isCalled.attach.arguments[0], document, '.attach function has context, document');
  assert.strictEqual(isCalled.attach.arguments[1].foo, true, '.attach function has settings.foo');
  assert.strictEqual(isCalled.attach.arguments[1].bar, false, '.attach function has settings.bar');
  assert.strictEqual(isCalled.attach.arguments[2], $, '.attach function has third argument, jQuery');
  assert.strictEqual(isCalled.ready.arguments[0], $, '.ready function has first argument, jQuery');
  assert.strictEqual(isCalled.ready.this.context, document, '.ready function has this.context document');
  assert.strictEqual(isCalled.ready.this.settings.foo, true, '.ready function has this.settings.foo');
  assert.strictEqual(isCalled.ready.this.settings.bar, false, '.ready function has this.settings.bar');

  // Mock call Drupal.detachBehaviors
  Drupal.behaviors.baz.detach(document, {foo: true, bar: false}, 'unload');

  assert.ok(isCalled.detach, 'detach is called');

  assert.strictEqual(isCalled.detach.arguments[0], document, '.detach function has context, document');
  assert.strictEqual(isCalled.detach.arguments[1].foo, true, '.detach function has settings.foo');
  assert.strictEqual(isCalled.detach.arguments[1].bar, false, '.detach function has settings.bar');
  assert.strictEqual(isCalled.detach.arguments[2], 'unload', '.detach function has third argument, unload');
  assert.strictEqual(isCalled.detach.arguments[3], $, '.detach function has fourth argument, jQuery');
});

}(jQuery));
