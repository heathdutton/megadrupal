
{mask:main}
<div class="node{if: {sticky} } sticky{/if}">
  {picture}
    {if: {page} == 0}<h2 class="title"><a href="{node_url}">{title}</a></h2>{/if}
    <span class="submitted">{submitted}</span>
    <div class="taxonomy">{terms}</div>
    <div class="content">{content}</div>
    {if: {links} }<div class="links">&raquo; {links}</div>{/if}
  </div>
{/mask}
