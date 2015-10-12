{* Smarty template parsed by CiviCRM Smarty *}
<div> {* privatecopy section starts *}
  <div class="crm-accordion-wrapper crm-report_privatecopy-accordion crm-accordion_title-accordion crm-accordion-closed">
    <div class="crm-accordion-header">
      <div class="icon crm-accordion-pointer"></div>
        {ts}Private Copy{/ts}
      </div><!-- /.crm-accordion-header -->
      <div class="crm-accordion-body">
        {if $is_private}
          {if $user_is_owner}
            <p>{ts}This report is private for your use only.{/ts}</p>
          {else}
            <p>{ts 1=$username}This report is private to <em>%1</em> only.{/ts}</p>
          {/if}
        {/if}
        <table class="form-layout-compressed" id="table_save_as">
        </table>
    </div><!-- /.crm-accordion-body -->
  </div><!-- /.crm-accordion-wrapper -->
</div> {* privatecopy section ends *}
