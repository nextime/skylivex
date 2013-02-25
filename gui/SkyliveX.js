function skyliveClass () {}
skyliveClass.prototype.method = function(name, func){
   this.prototype[name] = func;
   return this;
}
skyliveClass.method('changeContent', function(id, content){
   var n=document.getElementById(id);
   if(typeof(n)!="undefined")
     n.innerHTML=content;
});
skyliveClass.method('notify', function(content){
   if(typeof(notifycb)=="function")
      notifycb(content);
});
skyliveClass.method('msgalert', function(content){
   if(typeof(alertcb)=="function")
      alertcb(content);
   //else
   //   alert(content);
});     
skyliveClass.method('publicReceived', function(user, msg){
   if(typeof(public_received)=="function")
   {
      public_received(user, msg);
   } else {
      n = document.getElementById("chatarea");
      if(typeof(n)!="undefined")
      {
         var newel = document.createElement('p');
         newel.innerHTML="&lt;"+user+"&gt; "+msg;
         n.appendChild(newel);
         n.scollTop = n.scrollHeight;
      }  
   }  
});  
SkyliveX.page = new SkyliveClass();
SkyliveX.changeContent.connect(SkyliveX.page.changeContent);
SkyliveX.notify.connect(SkyliveX.page.notify);
SkyliveX.alertmsg.connect(SkyliveX.page.msgalert);
SkyliveX.public_received.connect(SkyliveX.page.publicReceived);
