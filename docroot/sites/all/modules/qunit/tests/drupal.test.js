
(function($) {

/**
 * Test the Drupal.checkPlain function.
 */
Drupal.tests.testCheckPlain = {
  getInfo: function() {
    return {
      name: 'Check plain',
      description: 'Tests the Drupal.checkPlain() JavaScript function for properly escaping HTML.',
      group: 'System'
    };
  },
  test: function() {
    expect(9);

    // Test basic strings.
    equals(Drupal.checkPlain('test'), 'test', Drupal.t("Nothing gets replaced that doesn't need to be replaced with their escaped equivalent."));
    equals(Drupal.checkPlain('"test'), '&quot;test', Drupal.t('Quotes are replaced with their escaped equivalent.'));
    equals(Drupal.checkPlain('Test&1'), 'Test&amp;1', Drupal.t('Ampersands are replaced with their escaped equivalent.'));
    equals(Drupal.checkPlain('Test>test'), 'Test&gt;test', Drupal.t('Greater-than signs are replaced with their escaped equivalent.'));
    equals(Drupal.checkPlain('Test<test'), 'Test&lt;test', Drupal.t('Less-than signs are replaced with their escaped equivalent.'));

    // Test other data types.
    equals(Drupal.checkPlain(['ampers&', 'q"ote']), 'ampers&amp;,q&quot;ote', Drupal.t('Arrays that need to have replacements have them done.'));
    equals(Drupal.checkPlain(1), '1', Drupal.t('Integers are left at their equivalent string value.'));

    // Combined tests.
    equals(Drupal.checkPlain('<tagname property="value">Stuff</tagname>'), '&lt;tagname property=&quot;value&quot;&gt;Stuff&lt;/tagname&gt;', Drupal.t('Full HTML tags are replaced with their escaped equivalent.'));
    equals(Drupal.checkPlain('Test "&".'), 'Test &quot;&amp;&quot;.', Drupal.t('A string with both quotes and ampersands replaces those entities with their escaped equivalents.'));
  }
};

/**
 * Tests Drupal.t().
 */
Drupal.tests.testT = {
  getInfo: function() {
    return {
      name: Drupal.t('Translation'),
      description: Drupal.t('Tests the basic translation functionality of the Drupal.t() function, including the proper handling of variable strings.'),
      group: Drupal.t('System')
    };
  },
  setup: function() {
    this.originalLocale = Drupal.locale;
    Drupal.locale = {
      'strings': {
        'Translation 1': '1 noitalsnarT',
        'Translation with a @placeholder': '@placeholder a with Translation',
        'Translation with another %placeholder': '%placeholder in another translation',
        'Literal !placeholder': 'A literal !placeholder',
        'Test unspecified placeholder': 'Unspecified placeholder test'
      }
    };
  },
  test: function() {
    expect(9);

    var html = '<tag attribute="value">Apples & Oranges</tag>';
    var escaped = '&lt;tag attribute=&quot;value&quot;&gt;Apples &amp; Oranges&lt;/tag&gt;';

    // Test placeholders.
    equals(Drupal.t('Hello world! @html', {'@html': html}), 'Hello world! ' + escaped, Drupal.t('The "@" placeholder escapes the variable.'));
    equals(Drupal.t('Hello world! %html', {'%html': html}), 'Hello world! <em class="placeholder">' + escaped + '</em>', Drupal.t('The "%" placeholder escapes the variable and themes it as a placeholder.'));
    equals(Drupal.t('Hello world! !html', {'!html': html}), 'Hello world! ' + html, Drupal.t('The "!" placeholder passes the variable through as-is.'));
    equals(Drupal.t('Hello world! html', {'html': html}), 'Hello world! <em class="placeholder">' + escaped + '</em>', Drupal.t('Other placeholders act as "%" placeholders do.'));

    // Test actual translations.
    equals(Drupal.t('Translation 1'), '1 noitalsnarT', Drupal.t('Basic translations work.'));
    equals(Drupal.t('Translation with a @placeholder', {'@placeholder': '<script>alert("xss")</script>'}), '&lt;script&gt;alert(&quot;xss&quot;)&lt;/script&gt; a with Translation', Drupal.t('Translations with the "@" placeholder work.'));
    equals(Drupal.t('Translation with another %placeholder', {'%placeholder': '<script>alert("xss")</script>'}), '<em class="placeholder">&lt;script&gt;alert(&quot;xss&quot;)&lt;/script&gt;</em> in another translation', Drupal.t('Translations with the "%" placeholder work.'));
    equals(Drupal.t('Literal !placeholder', {'!placeholder': '<script>alert("xss")</script>'}), 'A literal <script>alert("xss")</script>', Drupal.t('Translations with the "!" placeholder work.'));
    equals(Drupal.t('Test unspecified placeholder', {'placeholder': '<script>alert("xss")</script>'}), 'Unspecified <em class="placeholder">&lt;script&gt;alert(&quot;xss&quot;)&lt;/script&gt;</em> test', Drupal.t('Translations with unspecified placeholders work.'));
  },
  teardown: function() {

  }
};

/**
 * Tests Drupal.attachBehaviors().
 */
Drupal.tests.testBehaviors = {
  getInfo: function() {
    return {
      name: 'JavaScript behaviors',
      description: 'Tests the functionality of Drupal behaviors to make sure it allows JavaScript files to attach and detach behaviors in different contexts.',
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
          equals(context, 'Attach context ' + attachIndex, Drupal.t('Attach context matches passed context.'));
          equals(settings, 'Attach settings ' + attachIndex, Drupal.t('Attach settings match passed settings.'));
        },
        detach: function(context, settings) {
          detachIndex++;
          equals(context, 'Detach context ' + detachIndex, Drupal.t('Detach context matches passed context.'));
          equals(settings, 'Detach settings ' + detachIndex, Drupal.t('Detach settings match passed settings.'));
        }
      }
    };
  },
  test: function() {
    expect(8);

    // Test attaching behaviors.
    Drupal.attachBehaviors('Attach context 1', 'Attach settings 1');

    // Test attaching behaviors again.
    Drupal.attachBehaviors('Attach context 2', 'Attach settings 2');

    // Test detaching behaviors.
    Drupal.detachBehaviors('Detach context 1', 'Detach settings 1');

    // Try detaching behaviors again.
    Drupal.detachBehaviors('Detach context 2', 'Detach settings 2');
  },
  teardown: function() {
    Drupal.behaviors = this.originalBehaviors;
  }
};

})(jQuery);
