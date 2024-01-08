<img src="https://github.com/nightMoney74767/gamerZone/blob/main/LogoPNG.png" alt="Logo" width=500/>

# GamerZone++
This project contains the files for a game server written in C++ and Boost. To run this on your device, please follow these instructions:
<ol>
    <li>download the code from GitHub (link above)</li>
    <li>for Windows:</li>
    <ul>
      <li>download and run the installer for <a href="https://visualstudio.microsoft.com/downloads/">Visual Studio</a>. The community edition is available for free. When asked, select the c++ workload and click install</li>
      <li>visit the website for <a href="https://www.boost.org/users/download/">Boost</a> and select either the 7z or ZIP file</li>
      <li>extract the arhcive then open either the Command Prompt or Powershell (you can also use the Windows Terminal) and go to the directory where the archive was extracted (e.g., cd C:\Boost)</li>
      <li>run the <code>bootstrap</code> script. Once finished, run <code>.\b2</code></li>
      <li>download the project code and open the .sln file in Visual Studio</li>
      <li>in the solution explorer, right-click the Client folder and select properties</li>
      <li>select the C/C++ option in the left pane of the properties window. Set the additional include directories to point to the location of the Boost libraries (e.g., C:\Boost\boost_1_81_0\boost_1_81_0)</li>
      <li>click apply then go to the linker option in the left pane. Set the additional library directories to point to the same directory but with the addition of "\stage\lib"</li>
      <li>repeat the last two steps for the server</li>
      <li>to run both the client and server at the same time, click the drop-down arrow next to the "Local Windows Debugger" button and choose "configure startup projects". Click the "multiple projects" option and set both the client and server to start with or without debugging</li>
      <li>select the start button</li>
    </ul>
    <li>for macOS and Linux</li>
    <ul>
      <li>visit the website for <a href="https://www.boost.org/users/download/">Boost</a> and select either the TAR.GZ or the TAR.BZ2 file</li>
      <li>using an appropriate utility, extract the archive</li>
      <li>open a terminal and go to the directory where the libraries were extracted</li>
      <li>run the <code>bootstrap</code> script. Once finished, run <code>./b2</code></li>
      <li>compile the code in an IDE of your choice. Make sure the Boost libraries are included</li>
    </ul>
  </ol>
