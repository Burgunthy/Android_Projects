package edu.android.project.part2_chapter08_web

import android.annotation.SuppressLint
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.inputmethod.EditorInfo
import android.webkit.WebView
import android.webkit.WebViewClient
import android.widget.EditText

class MainActivity : AppCompatActivity() {

    private val addressBar: EditText by lazy {
        findViewById<EditText>(R.id.addressBar)
    }

    private val webView: WebView by lazy {
        findViewById<WebView>(R.id.webView)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        initViews()
        bindViews()
    }

    @SuppressLint("SetJavaScriptEnabled")
    private fun initViews() {
        webView.apply {
            webViewClient = WebViewClient()             // 이동되지 않게 하기
            settings.javaScriptEnabled = true
            loadUrl("http://www.google.com")        // 암호화되지 않았어!
        }
    }

    private fun bindViews() {
        // 액션이 발생했을 때
        addressBar.setOnEditorActionListener { v, actionId, event ->
            if(actionId == EditorInfo.IME_ACTION_DONE) {
                webView.loadUrl(v.text.toString())
            }

            return@setOnEditorActionListener false
        }
    }
}