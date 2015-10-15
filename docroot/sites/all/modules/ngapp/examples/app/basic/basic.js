/**
 * @file
 * Code for the ngApp module examples.
 */

(function() {
  'use strict';

  var ControllerBasic = ['drupalLocalStorage', 'drupalUser', '$rootScope', function(drupalLocalStorage, drupalUser, $rootScope) {
    // Bind this to vm (view-model).
    var vm = this;

    drupalUser
      .then(function(user) {
        if (user.uid > 0) {
          vm.message = 'Welcome ' + user.name + '!';
        }
        else {
          vm.message = "(Hey, you're not logged in!)";
        }
      });

    // Get thing from local storage.
    var thing = drupalLocalStorage.get('thing');
    if (!thing) {
      thing = '';
    }

    // Form submit action.
    vm.doThing = function(value) {
      drupalLocalStorage.set('thing', value);
      vm.thingActive = value;

      $rootScope
        .$emit('thingChanged', value);
    };

    // Init vm's thingActive binding.
    vm.thingActive = thing;
  }];

  // Bind component to globally scoped Angular module.
  $drupalApp
    .controller('ControllerBasic', ControllerBasic);

}());
