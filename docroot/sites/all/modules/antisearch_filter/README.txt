CONTENTS OF THIS FILE
---------------------

 * Synopsis
 * Configuration

SYNOPSIS
--------

The antisearch filter hides single words from search engines like Google, Bing,
Yahoo etc.

In this situation the antisearch filter module is the right module for you: Your
text on the internet has to found by search engines. Just a few words in it
should not be found under any circumstances.

How it works:

It adds random characters to the select string und hides this random characters
with CSS. Then the string "Josef Friedrich" for example is converted
to "J9ofs6e0fj 8Fcrpijerdgrtikcshj".

If the Antisearch Filter is activated, this example HTML code

<p><s>Josef Friedrich</s></p>

will be converted to:

<p><span class="antisearch-filter" title="Name is hidden from search engines.
Search engines see only: J9ofs6e0fj 8Fcrpijerdgrtikcshj">J<span
class="asf">9</span>o<span class="asf">f</span>s<span class="asf">6</span>e<span
class="asf">0</span>f<span class="asf">j</span> <span class="asf">8</span>F<span
class="asf">c</span>r<span class="asf">p</span>i<span class="asf">j</span>e<span
class="asf">r</span>d<span class="asf">g</span>r<span class="asf">t</span>i<span
class="asf">k</span>c<span class="asf">s</span>h<span
class="asf">j</span></span></p>

Configuration
-------------

Install the antisearch filter module and enable it. Go to Administration »
Configuration » Content authoring » Text formats
(admin/config/content/formats). Select the text format you want  to configure
and enable the antisearch filter for this text format.