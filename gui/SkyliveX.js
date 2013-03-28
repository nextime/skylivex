function skyliveClass () {}
skyliveClass.prototype.changeContent = function(id, content){
   var n=document.getElementById(id);
   if(typeof(n)!="undefined")
     n.innerHTML=content;
};
skyliveClass.prototype.notify = function(content){
   if(typeof(notifycb)=="function")
      notifycb(content);
};
skyliveClass.prototype.msgalert = function(content){
   if(typeof(alertcb)=="function")
      alertcb(content);
   //else
   //   alert(content);
};     
skyliveClass.prototype.publicReceived = function(user, msg){
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
         n.scrollTop = n.scrollHeight;
      }  
   }  
}; 
skyliveClass.prototype.userlistChange = function(action, u){
   if(typeof(userlist_change)=="function")
   {
      userlist_change(action, u);
   } else {
      switch(action)
      {
         case "new":
            break;
         case "add":
            break;
         case "del":
            break;
      }
   }
};

SkyliveXPage = new skyliveClass();
SkyliveX.changeContent.connect(SkyliveXPage.changeContent);
SkyliveX.notify.connect(SkyliveXPage.notify);
SkyliveX.alertmsg.connect(SkyliveXPage.msgalert);
SkyliveX.public_received.connect(SkyliveXPage.publicReceived);
SkyliveX.userlist_change.connect(SkyliveXPage.userlistChange);
