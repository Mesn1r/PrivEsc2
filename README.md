# PrivEsc2

ok so this time i will talk about Privilage escaltion technique which called Access Token manipulation 
so first of all , what is Access Token in Windows os and how it connect with the SE_DEBUG_NAME Privilege ?

Access Token is a data structure that describes the security context of a process or thread. It contains information about the user and security groups to which the user belongs, user privileges(!), and the user's security identifier (SID). access tokens enforce security restrictions and control access to resources .. On the other hand, SE_DEBUG_NAME is a privilege in Windows that allows the user to attach a debugger to any process running on the system. This privilege is used for debugging and development purposes and is granted to a user by adjusting their access token to include the SE_DEBUG_NAME privilege. When the SE_DEBUG_NAME privilege is enabled, the user can attach a debugger to any process running on the system as the access token is used by Windows to control access to resources and enforce security restrictions.

so how we get our access token ?! lets understand the duplication system ..  
duplication of access token in access control systems is a mechanishem to create a separate, impersonated access token that can be used to launch a new process with a different set of security attributes,This allows processes to be launched under different security contexts and helps enforce access restrictions and control access to resources.

ok the poc time - 
before we compailing the code we find the Process ID we want to take over in our case its winlogon.exe , why winlogon you ask ? good !

winlogon.exe responsible on critical parts in the logons and logoffs and privileges in general 
Privileges - 
* winlogon.exe runs with the SYSTEM account privileges, which is the highest privilege level in Windows.
communication with the kernel - 
* Winlogon.exe communicates with the Windows kernel to retrieve information about the system state, including the status of the logged-on user, and to initiate system shutdowns or restarts.

![Screenshot 2023-01-30 at 11 59 38](https://user-images.githubusercontent.com/122444563/215446067-94c1a087-8367-46af-b1e0-6debc02ca594.png)

![Screenshot 2023-01-30 at 12 06 17](https://user-images.githubusercontent.com/122444563/215447454-13dd0247-9ffc-4957-ac35-8df3515daa8e.png)

then let's compile the program 

and run as shown below : 


https://user-images.githubusercontent.com/122444563/215461901-5ec38d0d-e5f1-4b25-a9bf-764aa156179f.mp4



