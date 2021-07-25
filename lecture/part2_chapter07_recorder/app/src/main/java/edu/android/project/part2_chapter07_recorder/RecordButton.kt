package edu.android.project.part2_chapter07_recorder

import android.content.Context
import android.util.AttributeSet
import androidx.appcompat.widget.AppCompatImageButton

class RecordButton(
    context: Context,
    attrs: AttributeSet
): AppCompatImageButton(context, attrs) {
    // 상태에 따라 그림이 변경

    fun updateIconWithState(state: State) {
        when(state) {
            State.BEFORE_RECORDING -> {
                setImageResource(R.drawable.ic_record)
            }
            State.ON_RECORDING -> {
                setImageResource(R.drawable.ic_stop)
            }
            State.AFTER_RECORDING -> {
                setImageResource(R.drawable.ic_play)
            }
            State.ON_PLAYING -> {
                setImageResource(R.drawable.ic_stop)
            }
        }
    }
}