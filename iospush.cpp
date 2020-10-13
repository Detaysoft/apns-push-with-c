/*
 * iospush - HTTP/2 iOS Push Request Application
 *
 * Copyright (c) 2020 Detay Teknoloji
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <iostream>
#include <string.h>

#include <nghttp2/asio_http2_client.h>

using namespace std;
using boost::asio::ip::tcp;

using namespace nghttp2::asio_http2::client;

int main(int argc, char* argv[]) {
    string deviceKey ("-device");
    string messageKey ("-message");
    string jwtKey ("-jwt");
    string bundleIDKey ("-bundle");
    string titleKey ("-title");
    string tagKey ("-tag");

    string device;
    string message;
    string jwt;
    string bundleID;
    string title;
    string tag;

    // set arguments
    for (int i = 0; i < argc; i++) {
        if (strcmp(deviceKey.c_str(), *&argv[i]) == 0) {
            device = *&argv[i+1];
        }
        else if (strcmp(messageKey.c_str(), *&argv[i]) == 0) {
            message = *&argv[i+1];
        }
        else if (strcmp(jwtKey.c_str(), *&argv[i]) == 0) {
            jwt = *&argv[i+1];
        }
        else if (strcmp(bundleIDKey.c_str(), *&argv[i]) == 0) {
            bundleID = *&argv[i+1];
        }
        else if (strcmp(titleKey.c_str(), *&argv[i]) == 0) {
            title = *&argv[i+1];
        }
        else if (strcmp(tagKey.c_str(), *&argv[i]) == 0) {
            tag = *&argv[i+1];
        }
    }

    // check variables
    if (bundleID.empty() ||
        jwt.empty() ||
        message.empty() ||
        device.empty()) {
        cout << "bundleID, jwt, message and device is cannot be empty";
        throw exception();
    }

    boost::system::error_code ec;
    boost::asio::io_service io_service;

    boost::asio::ssl::context tls(boost::asio::ssl::context::sslv23);
    tls.set_default_verify_paths();
    // disabled to make development easier...
    // tls_ctx.set_verify_mode(boost::asio::ssl::verify_peer);
    configure_tls_context(ec, tls);

    // connect to"api.push.apple.com:443"
    session sess(io_service, tls, "api.push.apple.com", "443");

    sess.on_connect([&sess, &jwt, &device, &message, &bundleID, &title, &tag](tcp::resolver::iterator endpoint_it) {
        boost::system::error_code ec;

        nghttp2::asio_http2::header_map headers;
        headers.emplace("authorization", (nghttp2::asio_http2::header_value){ "Bearer " + jwt });
        headers.emplace("apns-priority", (nghttp2::asio_http2::header_value){ "10" });
        headers.emplace("apns-topic", (nghttp2::asio_http2::header_value){ bundleID });
        headers.emplace("apns-push-type", (nghttp2::asio_http2::header_value){ "alert" });
        headers.emplace("apns-expiration", (nghttp2::asio_http2::header_value){ "0" });
        headers.emplace("content-type", (nghttp2::asio_http2::header_value){ "application/json" });

        priority_spec prio = priority_spec();

        auto req = sess.submit(
           ec, "POST", "https://api.push.apple.com/3/device/" + device,
           "{\"aps\": {\"alert\": \"" + message + "\", \"badge\": 1}, \"title\": \"" + title + "\", \"body\": \"" + message + "\", \"tag\": \"" + tag + "\"}",
           headers,
           prio
        );

        req->on_response([](const response &res) {
            // print status code and response header fields.
            cout << "HTTP/2 " << res.status_code() << endl;
            for (auto &kv : res.header()) {
                cout << kv.first << ": " << kv.second.value << "\n";
            }
            cout << endl;

            res.on_data([](const uint8_t *data, size_t len) {
                cout.write(reinterpret_cast<const char *>(data), len);
                cout << endl;
            });
        });

        req->on_close([&sess](uint32_t error_code) {
            // shutdown session after first request was done.
            sess.shutdown();
        });
    });

    sess.on_error([](const boost::system::error_code &ec) {
        cout << "error: " << ec.message() << endl;
    });

    io_service.run();
}
