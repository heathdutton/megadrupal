(function ($) {

/**
 * Tests Drupal.checkPlain().
 */
Drupal.tests.checkPlain = {
  getInfo: function() {
    return {
      name: 'Drupal.checkPlain()',
      description: 'Tests for Drupal.checkPlain().',
      group: 'System'
    };
  },
  tests: {
    basicstring: function ($, Drupal) {
      return function() {
        expect(5);

        equal(Drupal.checkPlain('test'), 'test', Drupal.t("Nothing gets replaced that doesn't need to be replaced with their escaped equivalent."));
        equal(Drupal.checkPlain('"test'), '&quot;test', Drupal.t('Quotes are replaced with their escaped equivalent.'));
        equal(Drupal.checkPlain('Test&1'), 'Test&amp;1', Drupal.t('Ampersands are replaced with their escaped equivalent.'));
        equal(Drupal.checkPlain('Test>test'), 'Test&gt;test', Drupal.t('Greater-than signs are replaced with their escaped equivalent.'));
        equal(Drupal.checkPlain('Test<test'), 'Test&lt;test', Drupal.t('Less-than signs are replaced with their escaped equivalent.'));
      }
    },
    testother: function ($, Drupal) {
      return function() {
        expect(2);

        equal(Drupal.checkPlain(['ampers&', 'q"ote']), 'ampers&amp;,q&quot;ote', Drupal.t('Arrays that need to have replacements have them done.'));
        equal(Drupal.checkPlain(1), '1', Drupal.t('Integers are left at their equivalent string value.'));
      }
    },
    testcombined: function ($, Drupal) {
      return function() {
        expect(2);

        equal(Drupal.checkPlain('<tagname property="value">Stuff</tagname>'), '&lt;tagname property=&quot;value&quot;&gt;Stuff&lt;/tagname&gt;', Drupal.t('Full HTML tags are replaced with their escaped equivalent.'));
        equal(Drupal.checkPlain('Test "&".'), 'Test &quot;&amp;&quot;.', Drupal.t('A string with both quotes and ampersands replaces those entities with their escaped equivalents.'));
      }
    }
  }
};

/**
 * Tests Drupal.t().
 */
Drupal.tests.drupalt = {
  getInfo: function() {
    return {
      name: 'Drupal.t()',
      description: 'Tests for Drupal.t().',
      group: 'System'
    };
  },
  setup: function() {
    this.originalLocale = Drupal.locale;
    Drupal.locale = Drupal.locale || {};
    Drupal.locale.strings = Drupal.locale.strings || {};
    Drupal.locale = {
      'strings': {
        '' : {
          'Translation 1': '1 noitalsnarT',
          'Translation with a @placeholder': '@placeholder a with Translation',
          'Translation with another %placeholder': '%placeholder in another translation',
          'Literal !placeholder': 'A literal !placeholder',
          'Test unspecified placeholder': 'Unspecified placeholder test'
        }
      }
    };
  },
  teardown: function() {
    // Drupal.locale = this.originalLocale;
  },
  tests: {
    placeholders: function ($, Drupal) {
      return function() {
        expect(4);

        var html = '<tag attribute="value">Apples & Oranges</tag>';
        var escaped = '&lt;tag attribute=&quot;value&quot;&gt;Apples &amp; Oranges&lt;/tag&gt;';

        equal(Drupal.t('Hello world! @html', {'@html': html}), 'Hello world! ' + escaped, Drupal.t('The "@" placeholder escapes the variable.'));
        equal(Drupal.t('Hello world! %html', {'%html': html}), 'Hello world! <em class="placeholder">' + escaped + '</em>', Drupal.t('The "%" placeholder escapes the variable and themes it as a placeholder.'));
        equal(Drupal.t('Hello world! !html', {'!html': html}), 'Hello world! ' + html, Drupal.t('The "!" placeholder passes the variable through as-is.'));
        equal(Drupal.t('Hello world! html', {'html': html}), 'Hello world! <em class="placeholder">' + escaped + '</em>', Drupal.t('Other placeholders act as "%" placeholders do.'));
      }
    },
    translations: function ($, Drupal) {
      return function() {
        expect(5);

        equal(Drupal.t('Translation 1'), '1 noitalsnarT', Drupal.t('Basic translations work.'));
        equal(Drupal.t('Translation with a @placeholder', {'@placeholder': '<script>alert("xss")</script>'}), '&lt;script&gt;alert(&quot;xss&quot;)&lt;/script&gt; a with Translation', Drupal.t('Translations with the "@" placeholder work.'));
        equal(Drupal.t('Translation with another %placeholder', {'%placeholder': '<script>alert("xss")</script>'}), '<em class="placeholder">&lt;script&gt;alert(&quot;xss&quot;)&lt;/script&gt;</em> in another translation', Drupal.t('Translations with the "%" placeholder work.'));
        equal(Drupal.t('Literal !placeholder', {'!placeholder': '<script>alert("xss")</script>'}), 'A literal <script>alert("xss")</script>', Drupal.t('Translations with the "!" placeholder work.'));
        equal(Drupal.t('Test unspecified placeholder', {'placeholder': '<script>alert("xss")</script>'}), 'Unspecified <em class="placeholder">&lt;script&gt;alert(&quot;xss&quot;)&lt;/script&gt;</em> test', Drupal.t('Translations with unspecified placeholders work.'));
      }
    }
  }
};

/**
 * Tests Drupal.attachBehaviors().
 */
Drupal.tests.drupalt = {
  getInfo: function() {
    return {
      name: 'Drupal.attachBehaviors()',
      description: 'Tests for Drupal.attachBehaviors().',
      group: 'System'
    };
  },
  setup: function() {
    this.originalBehaviors = Drupal.behaviors;
    var attachIndex = 0;
    var detachIndex = 0;
    Drupal.behaviors = {
      testBehavior: {
        attach: function(context, settings) {
          attachIndex++;
          equal(context, 'Attach context ' + attachIndex, Drupal.t('Attach context matches passed context.'));
          equal(settings, 'Attach settings ' + attachIndex, Drupal.t('Attach settings match passed settings.'));
        },
        detach: function(context, settings) {
          detachIndex++;
          equal(context, 'Detach context ' + detachIndex, Drupal.t('Detach context matches passed context.'));
          equal(settings, 'Detach settings ' + detachIndex, Drupal.t('Detach settings match passed settings.'));
        }
      }
    };
  },
  teardown: function() {
    Drupal.behaviors = this.originalBehaviors;
  },
  tests: {
    behaviours: function ($, Drupal) {
      return function() {
        expect(8);

        // Test attaching behaviors.
        Drupal.attachBehaviors('Attach context 1', 'Attach settings 1');

        // Test attaching behaviors again.
        Drupal.attachBehaviors('Attach context 2', 'Attach settings 2');

        // Test detaching behaviors.
        Drupal.detachBehaviors('Detach context 1', 'Detach settings 1');

        // Try detaching behaviors again.
        Drupal.detachBehaviors('Detach context 2', 'Detach settings 2');
      }
    }
  }
};

})(jQuery);