<!DOCTYPE html>
<html>
  <head>
    <title>Disease Dataset Curation | Log in</title>
    <script src="https://www.google.com/jsapi"></script>
    <script src="/assets/chartkick-15060e7f3e7ef844d90d1d86eca3ebf4b68379956c8ac68b0d203159187beef8.js"></script>
    <link rel="stylesheet" media="all" href="/assets/application-2df8aa154a82eff1312357c34f09cfb2e59e588bfb8b636cbf52acaa047c73f7.css" data-turbolinks-track="true" />
    <script src="/assets/application-8c9a2380b3afc6950fc8a356afbd982a37a8f845a610bf663cba940399645fbb.js" data-turbolinks-track="true"></script>
    <meta name="csrf-param" content="authenticity_token" />
<meta name="csrf-token" content="IOumSP3BdxG2kPju6s9dXQy3ySE/WqJvBQPvCwwP9oKwQCY0bzJ8mK8boM+ch9M9sFQgTknBI7r6Khd6HKmD+A==" />
    <!--[if lt IE 9]>
      <script src="//cdnjs.cloudflare.com/ajax/libs/html5shiv/r29/html5.min.js">
      </script>
    <![endif]-->
  </head>
  <body>
      <header class="navbar navbar-fixed-top navbar-inverse">
  <div class="container">
    <a id="logo" href="/">Genetic Dataset Curation</a>
    <nav>
      <ul class="nav navbar-nav navbar-right">
        <li>
            <a href="/">Home</a>
        </li>
        <li><a href="#">Help</a></li>
          <li><a href="/login">Log in</a></li>
      </ul>
    </nav>
  </div>
</header>

      <div class="container">
        <h1>Log in</h1>

<div class="row">
  <div class="col-md-6 col-md-offset-3">
    <form action="/login" accept-charset="UTF-8" method="post"><input name="utf8" type="hidden" value="&#x2713;" /><input type="hidden" name="authenticity_token" value="jq5VXLE+aRmowJRRX3ScvZLzl/sN7E3rmsfNPmAbtjYeBdUgI81ikLFLzHApPBLdLhB+lHt3zD5l7jVPcL3DTA==" />

      <label for="session_email">Email</label>
      <input class="form-control" type="email" name="session[email]" id="session_email" />

      <label for="session_password">Password</label>
      <input class="form-control" type="password" name="session[password]" id="session_password" />

      <label class="checkbox inline" for="session_remember_me">
        <input name="session[remember_me]" type="hidden" value="0" /><input type="checkbox" value="1" name="session[remember_me]" id="session_remember_me" />
        <span>Remember me on this computer</span>
</label>
      <input type="submit" name="commit" value="Log in" class="btn btn-primary" />
</form>
    <p>New user? <a href="/signup">Sign up now!</a></p>
  </div>
</div>

<!DOCTYPE html>
<html>
<head>
  <title>Facebook Auth Example</title>
  <link rel="stylesheet" media="all" href="/assets/application-2df8aa154a82eff1312357c34f09cfb2e59e588bfb8b636cbf52acaa047c73f7.css" data-turbolinks-track="true" />
  <script src="/assets/application-8c9a2380b3afc6950fc8a356afbd982a37a8f845a610bf663cba940399645fbb.js" data-turbolinks-track="true"></script>
  <meta name="csrf-param" content="authenticity_token" />
<meta name="csrf-token" content="Po+9bq6uxwscwGIxCV7BxBZ8ez/fLDGOSV1CsmzR2rGuJD0SPF3MggVLOhB/Fk+kqp+SUKm3sFu2dLrDfHevyw==" />
</head>
<body>
<div>
    <a id="sign_in" href="/auth/facebook">Sign in with Facebook</a>
</div>


</body>
</html>
<!DOCTYPE html>
<html>
  <head>
    <title>Google Auth Example App</title>
    <link rel="stylesheet" media="all" href="/assets/application-2df8aa154a82eff1312357c34f09cfb2e59e588bfb8b636cbf52acaa047c73f7.css" data-turbolinks-track="true" />
    <script src="/assets/application-8c9a2380b3afc6950fc8a356afbd982a37a8f845a610bf663cba940399645fbb.js" data-turbolinks-track="true"></script>
    <meta name="csrf-param" content="authenticity_token" />
<meta name="csrf-token" content="zMA+eh6pYNI+Nq6d49IkvgomPv8tI+aoKrd8KxCn739ca74GjFprWye99ryVmqretsXXkFu4Z33VnoRaAAGaBQ==" />
  </head>
  <body>
    <div>
        <a id="sign_in" href="/auth/google_oauth2">Sign in with Google</a>
    </div>
    <div>
    
    </div>
  </body>
</html>


        <footer class="footer">
  <small>
    The <a href="https://github.com/xvxiaopei/CSCE606-project">Dataset Curation</a>
    by <a href="https://github.com/xvxiaopei/CSCE606-project">Team Flyer and 666</a>
  </small>
  <nav>
    <ul>
      <li><a href="#">About</a></li>
      <li><a href="#">Contact</a></li>
    </ul>
  </nav>
</footer>

        
      </div>
  </body>
</html>

