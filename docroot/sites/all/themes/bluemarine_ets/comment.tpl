
{mask:main}
<div class="comment {status}">
  {picture}
<h3 class="title">{title}</h3>
  {if: {new} }<span class="new">{new}</span>{/if}
    <div class="submitted">{submitted}</div>
    <div class="content">
     {content}
     {if: {signature} }
      <div class="clear-block">
       <div>—</div>
       {signature}
      </div>
     {/if}
    </div>
    {if: {links} }<div class="links">&raquo; {links}</div>{/if}
  </div>
{/mask}
