#include <iostream>
#include <fstream>

#include "./httplib.h"

using namespace std;

int main(int argc, char *argv[])
{
    const int PORT = 21543;
    const string HOST = "0.0.0.0";
    httplib::Server svr;

    svr.set_error_handler([](const auto &req, auto &res)
                          {
        auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
        char buf[BUFSIZ];
        snprintf(buf, sizeof(buf), fmt, res.status);
        res.set_content(buf, "text/html"); });

    svr.set_default_headers({{"Access-Control-Allow-Origin", "*"},
                             {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
                             {"Access-Control-Allow-Headers", "*"}});

    // Mount /public to ./ directory
    auto ret = svr.set_mount_point("/public", "./");
    if (!ret)
    {
        cerr << "Failed to set mount point" << endl;
        return 1;
    }

    svr.Options("(.*)", [](const httplib::Request &req, httplib::Response &res) {});

    svr.Post("/record", [](const httplib::Request &req, httplib::Response &res)
             { 
                string recordFileName = "rrweb_" + to_string(time(NULL)) + ".json";
        ofstream jsonFile;
        jsonFile.open (recordFileName);
        jsonFile << req.body << "\n";
        jsonFile.close();
        
        res.set_content(recordFileName, "text/plain"); });

    svr.Get("/replay", [](const httplib::Request &req, httplib::Response &res)
            {
                string html = R"(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link
    rel="stylesheet"
    href="https://cdn.jsdelivr.net/npm/rrweb-player@latest/dist/style.css"
  />
  <script src="https://cdn.jsdelivr.net/npm/rrweb-player@latest/dist/index.js"></script>
  <title>rrweb-server</title>
  <link rel="icon" href="data:image/svg+xml,<svg xmlns=%22http://www.w3.org/2000/svg%22 viewBox=%220 0 100 100%22><text y=%22.9em%22 font-size=%2290%22>▶️</text></svg>">
  <style>
    html,
    body {
    margin: 0;
    padding: 0;
    font-family: sans-serif;
    }

    form {
    display: flex;
    flex-direction: column;
    gap: 10px;
    padding: 10px;
    }

    hr {
    width: 90%;
    }

    #root {
    width: 100%;
    display: flex;
    flex-direction: column;
    align-items: center;
    gap: 10px;
    }
  </style>
</head>
<body>
  <div id="root">
    <form>
      <label for="file">Select a events .json file:</label>
      <input type="file" id="file" accept=".json" />
    </form>
    <hr />
    <div id="player"></div>
  </div>
  <script>
    function handleFileSelect(evt) {
      // first remove all #player children
      while (document.querySelector('#player').firstChild) {
        document.querySelector('#player').removeChild(document.querySelector('#player').firstChild);
      }
      var files = evt.target.files; // FileList object
      var f = files[0]; // The first file
      var reader = new FileReader();
      reader.onload = function(event) {
        var data = JSON.parse(event.target.result);
        new rrwebPlayer({
          target: document.querySelector('#player'),
          props: {
            events: data,
            autoPlay: false,
            width: 600,
            height: 400
          },
        });
      };
      reader.readAsText(f);
    }
    document.getElementById('file').addEventListener('change', handleFileSelect, false);
  </script>
</body>
</html>)";
                res.set_content(html, "text/html"); });

    cout << "Server listening on http://" << HOST << ":" << PORT << endl;

    svr.listen(HOST, PORT);

    return 0;
}
