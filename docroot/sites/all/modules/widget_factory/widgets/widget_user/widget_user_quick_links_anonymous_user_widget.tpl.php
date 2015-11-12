<ul class="nav navbar-top-links navbar-right">
  <li class="dropdown">
      <a class="dropdown-toggle" data-toggle="dropdown" href="#" aria-expanded="false">
          <i class="fa fa-user fa-fw"></i>  <i class="fa fa-caret-down"></i>
      </a>
      <ul class="dropdown-menu dropdown-user">
          <li><a href="/user/login"><i class="fa fa-user fa-fw"></i> User Login</a>
            <div class="well-sm">
              <?php echo render($login_form); ?>
            </div>
          </li>
          <li class="divider"></li>
          <li><a href="/user/register"><i class="fa fa-gear fa-fw"></i> Create new account</a>
          </li>
          <li class="divider"></li>
          <li><a href="/user/password"><i class="fa fa-question fa-fw"></i> Forgot Password?</a>
          </li>
      </ul>
      <!-- /.dropdown-user -->
  </li>
</ul>
