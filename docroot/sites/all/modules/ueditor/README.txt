
DESCRIPTION
----------------
UEditor(百度编辑器) is Baidu web front-end R & D department developed
WYSIWYG rich text web editor, with a lightweight, customizable,
and focus on user experience and other characteristics,
the open source BSD license, allowing free use and modify the code.

I have used many editors, but ultimately I think UEditor is the 
best, so I want more people to know and use it.

if you have any question about this module, please create a 
issue and let me know, I will always always always ...keep 
actively maintained, enjoy!

Demo: http://ueditor.baidu.com/website/onlinedemo.html
The demo is chinese, You can use the English version on your download.

Installation
-------------
1. Download the ueditor in
http://ueditor.baidu.com/website/index.html.
2. Unzip it into sites/all/libraries,
so that there's like sites/all/libraries/ueditor/ueditor.all.js.
3. Do like this https://drupal.org/node/2286333
4. Enabled ueditor module.
5. Go to admin/people/permissions and grant permission to any roles that need to be 
able to upload file with ueditor.
6. Add ueditor for Wysiwyg profile at admin/config/content/wysiwyg 
7. Configure the module at admin/config/content/ueditor (Optional)

Known Issues
-----------------
https://drupal.org/node/2286333
https://drupal.org/node/2286537

Form Textarea Field
-----------------------

if you want to use ueditor to your custom form with form API, there is 
a simple example:

Form:

function MY_MODULE_example_form($form, &$form_state){
  $form['ueditor_example'] = array(
    '#type' => 'text_format',
    '#title' => 'Ueditor Example',
    '#format' => 'full_html',
  );
}

Form submit:

function MY_MODULE_example_form_submit($form, &$form_state){
  preg_match_all('/(.*?(src|href)=["|\'])(.*?)["|\']/ms',
  $form_state['values']['ueditor_example']['value'], $matchs);
  if(isset($matchs[3])){
    ueditor_file_register($matchs[3], $form_state['values']['form_id'], 'ueditor_example', '');       
  }
  ...

  Do some thing

  ...
}

Tips: The official website of the Chinese,
You can use the google translation, the operation so easy.
