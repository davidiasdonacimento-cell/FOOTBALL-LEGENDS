package com.footballlegends.mobile;

import android.app.AlertDialog;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;

/** Compact match HUD: analog movement, contextual actions and a separate referee strip. */
public final class MatchUi extends FrameLayout {
    private final GameSurface surface;
    private final Context context;
    private final TextView modeLabel;
    private int role = 0;
    private int selectedPlayer = 10;
    private boolean whistlePaused;
    private Runnable exitAction;

    public MatchUi(Context context, GameSurface surface) {
        super(context);
        this.context = context;
        this.surface = surface;
        setClipChildren(false);
        setClipToPadding(false);
        setClickable(false);

        LinearLayout top = new LinearLayout(context);
        top.setGravity(Gravity.CENTER_VERTICAL);
        top.setPadding(dp(10), dp(4), dp(6), dp(4));
        top.setBackground(panel());
        modeLabel = label("JOGADOR", 13, true);
        top.addView(modeLabel, new LinearLayout.LayoutParams(0, dp(42), 1));
        Button mode = button("ÁRBITRO");
        mode.setOnClickListener(v -> setRole(role == 0 ? 1 : 0));
        top.addView(mode, new LinearLayout.LayoutParams(dp(94), dp(40)));
        Button pause = button("Ⅱ");
        pause.setOnClickListener(v -> showPauseMenu());
        top.addView(pause, new LinearLayout.LayoutParams(dp(48), dp(40)));
        LayoutParams topLp = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT, Gravity.TOP);
        topLp.setMargins(dp(8), dp(8), dp(8), 0);
        addView(top, topLp);
        setRole(0);
    }

    public void setExitAction(Runnable action) { exitAction = action; }

    public void setRole(int newRole) {
        role = newRole == 1 ? 1 : 0;
        surface.setControlRole(role);
        modeLabel.setText(role == 0 ? "PARTIDA  •  JOGADOR" : "PARTIDA  •  ÁRBITRO");
        removeViewsBelowTop();
        if (role == 0) buildPlayerHud(); else buildRefereeHud();
    }

    private void removeViewsBelowTop() {
        while (getChildCount() > 1) removeViewAt(1);
    }

    private void buildPlayerHud() {
        Joystick joystick = new Joystick(context);
        LayoutParams joyLp = new LayoutParams(dp(148), dp(148), Gravity.BOTTOM | Gravity.START);
        joyLp.setMargins(dp(18), 0, 0, dp(20));
        addView(joystick, joyLp);

        LinearLayout actions = new LinearLayout(context);
        actions.setOrientation(LinearLayout.VERTICAL);
        actions.setGravity(Gravity.CENTER);
        actions.setPadding(dp(7), dp(7), dp(7), dp(7));
        actions.setBackground(panel());
        Button shoot = actionButton("CHUTE", 1, dp(76), dp(66));
        shoot.setTextSize(13);
        actions.addView(shoot);
        LinearLayout row = new LinearLayout(context);
        row.setGravity(Gravity.CENTER);
        row.addView(actionButton("PASSE", 0, dp(66), dp(54)));
        row.addView(actionButton("DRIBLE", 2, dp(66), dp(54)));
        actions.addView(row);
        LinearLayout bottom = new LinearLayout(context);
        bottom.setGravity(Gravity.CENTER);
        bottom.addView(holdActionButton("CORRER", 3, dp(66), dp(48)));
        bottom.addView(actionButton("DESARME", 4, dp(66), dp(48)));
        actions.addView(bottom);
        Button more = button("MAIS ▴");
        more.setTextSize(9);
        more.setOnClickListener(v -> showExtraActions());
        actions.addView(more, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, dp(34)));
        LayoutParams actionLp = new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, Gravity.BOTTOM | Gravity.END);
        actionLp.setMargins(0, 0, dp(12), dp(16));
        addView(actions, actionLp);
    }

    private void showExtraActions() {
        new AlertDialog.Builder(context).setTitle("AÇÕES DA PARTIDA")
            .setItems(new String[]{"SALTAR / DISPUTAR BOLA", "CABECEAR", "GOLEIRO: MERGULHO / DEFESA", "GOLEIRO: SEGURAR BOLA", "TROCAR JOGADOR", "DIFICULDADE DA IA"},
                (dialog, which) -> {
                    if (which == 0) surface.playerAction(8, -1, 1f, 0f);
                    else if (which == 1) surface.playerAction(9, -1, 1f, 0f);
                    else if (which == 2) surface.playerAction(5, -1, 1f, 0f);
                    else if (which == 3) surface.playerAction(6, -1, 1f, 0f);
                    else if (which == 4) showPlayerSelector();
                    else showDifficultySelector();
                }).show();
    }

    private void showPlayerSelector() {
        String[] names = new String[11];
        names[0] = "GOLEIRO • CAMISA 1";
        for (int i = 1; i < 11; i++) names[i] = "JOGADOR • CAMISA " + (i + 1);
        new AlertDialog.Builder(context).setTitle("CONTROLAR JOGADOR")
            .setSingleChoiceItems(names, selectedPlayer, (d, which) -> {
                selectedPlayer = which;
                surface.selectPlayer(which);
                d.dismiss();
            }).setNegativeButton("FECHAR", null).show();
    }

    private void showDifficultySelector() {
        String[] values = {"FÁCIL", "NORMAL", "DIFÍCIL"};
        int current = context.getSharedPreferences("football_legends_settings", Context.MODE_PRIVATE).getInt("ai_difficulty", 1);
        new AlertDialog.Builder(context).setTitle("DIFICULDADE DA IA")
            .setSingleChoiceItems(values, Math.max(0, Math.min(2, current)), (d, which) -> {
                surface.setAIDifficulty(which);
                context.getSharedPreferences("football_legends_settings", Context.MODE_PRIVATE).edit().putInt("ai_difficulty", which).apply();
                d.dismiss();
            }).setNegativeButton("FECHAR", null).show();
    }

    private void buildRefereeHud() {
        LinearLayout panel = new LinearLayout(context);
        panel.setOrientation(LinearLayout.VERTICAL);
        panel.setPadding(dp(8), dp(6), dp(8), dp(6));
        panel.setBackground(panel());
        Spinner players = new Spinner(context);
        String[] options = new String[22];
        for (int i = 0; i < 22; i++) options[i] = (i < 11 ? "AZUL" : "VERMELHO") + " • CAMISA " + ((i % 11) + 1);
        players.setAdapter(new ArrayAdapter<>(context, android.R.layout.simple_spinner_dropdown_item, options));
        players.setSelection(0);
        players.setOnItemSelectedListener(new android.widget.AdapterView.OnItemSelectedListener() {
            @Override public void onItemSelected(android.widget.AdapterView<?> parent, View view, int position, long id) { selectedPlayer = position; }
            @Override public void onNothingSelected(android.widget.AdapterView<?> parent) { }
        });
        panel.addView(players, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, dp(42)));
        LinearLayout cards = new LinearLayout(context);
        Button whistle = button("APITAR");
        whistle.setOnClickListener(v -> {
            if (whistlePaused) surface.resumeMatch(); else surface.pauseMatch();
            whistlePaused = !whistlePaused;
            whistle.setText(whistlePaused ? "RETOMAR" : "APITAR");
        });
        Button yellow = button("AMARELO");
        yellow.setOnClickListener(v -> surface.refereeIssueCard(selectedPlayer, 1));
        Button red = button("VERMELHO");
        red.setOnClickListener(v -> surface.refereeIssueCard(selectedPlayer, 2));
        cards.addView(whistle, new LinearLayout.LayoutParams(0, dp(44), 1));
        cards.addView(yellow, new LinearLayout.LayoutParams(0, dp(44), 1));
        cards.addView(red, new LinearLayout.LayoutParams(0, dp(44), 1));
        panel.addView(cards);
        Button decisions = button("FALTA  •  VANTAGEM  •  IMPEDIMENTO  •  VAR");
        decisions.setOnClickListener(v -> new AlertDialog.Builder(context).setTitle("DECISÕES DO ÁRBITRO")
            .setItems(new String[]{"Marcar falta", "Dar vantagem", "Marcar impedimento", "Solicitar VAR", "VAR: manter decisão", "VAR: anular decisão", "Cobrar reinício"},
                (d, which) -> {
                    if (which == 0) surface.refereeCallFoul(selectedPlayer);
                    else if (which == 1) surface.refereePlayAdvantage(selectedPlayer);
                    else if (which == 2) surface.refereeCallOffside(selectedPlayer);
                    else if (which == 3) surface.refereeRequestVar();
                    else if (which == 4) surface.refereeResolveVar(true);
                    else if (which == 5) surface.refereeResolveVar(false);
                    else surface.refereeRestartPlay();
                }).show());
        panel.addView(decisions, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, dp(42)));
        LayoutParams lp = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT, Gravity.BOTTOM);
        lp.setMargins(dp(8), 0, dp(8), dp(8));
        addView(panel, lp);
    }

    private Button actionButton(String text, int action, int width, int height) {
        Button b = button(text);
        b.setTextSize(10);
        b.setOnClickListener(v -> surface.playerAction(action, -1, 1f, 0f));
        b.setLayoutParams(new LinearLayout.LayoutParams(width, height));
        return b;
    }

    private Button holdActionButton(String text, int action, int width, int height) {
        Button b = button(text);
        b.setTextSize(10);
        b.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_DOWN) surface.playerAction(action, -1, 1f, 0f);
            else if (event.getAction() == MotionEvent.ACTION_UP || event.getAction() == MotionEvent.ACTION_CANCEL)
                surface.playerAction(3, -1, 0f, 0f);
            return true;
        });
        b.setLayoutParams(new LinearLayout.LayoutParams(width, height));
        return b;
    }

    private final class Joystick extends View {
        private final Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        private float centerX, centerY, knobX, knobY, radius;
        private boolean active;
        Joystick(Context c) { super(c); setLayerType(View.LAYER_TYPE_SOFTWARE, null); }
        @Override protected void onDraw(Canvas canvas) {
            super.onDraw(canvas);
            centerX = getWidth() * .5f; centerY = getHeight() * .5f;
            radius = Math.min(getWidth(), getHeight()) * .43f;
            if (!active) { knobX = centerX; knobY = centerY; }
            paint.setColor(0x66304050); paint.setStyle(Paint.Style.FILL);
            canvas.drawCircle(centerX, centerY, radius, paint);
            paint.setColor(0xCC20C7D8); paint.setStyle(Paint.Style.STROKE); paint.setStrokeWidth(dp(3));
            canvas.drawCircle(centerX, centerY, radius, paint);
            paint.setColor(0xEE0B6D91); paint.setStyle(Paint.Style.FILL);
            canvas.drawCircle(knobX, knobY, radius * .43f, paint);
            paint.setColor(Color.WHITE); paint.setTextAlign(Paint.Align.CENTER); paint.setTextSize(dp(10));
            canvas.drawText("MOVIMENTO", centerX, getHeight() - dp(5), paint);
        }
        @Override public boolean onTouchEvent(MotionEvent event) {
            if (event.getAction() == MotionEvent.ACTION_DOWN || event.getAction() == MotionEvent.ACTION_MOVE) {
                active = true;
                float dx = event.getX() - centerX, dy = event.getY() - centerY;
                float length = (float)Math.sqrt(dx * dx + dy * dy);
                float limit = radius * .78f;
                if (length > limit) { dx *= limit / length; dy *= limit / length; }
                knobX = centerX + dx; knobY = centerY + dy;
                surface.movePlayer(dx / limit, -dy / limit);
                invalidate();
                return true;
            }
            if (event.getAction() == MotionEvent.ACTION_UP || event.getAction() == MotionEvent.ACTION_CANCEL) {
                active = false;
                surface.movePlayer(0f, 0f);
                invalidate();
                return true;
            }
            return true;
        }
    }

    private void showPauseMenu() {
        surface.pauseMatch();
        whistlePaused = true;
        String[] options = {"CONTINUAR", "MODO ÁRBITRO", "MODO JOGADOR", "VELOCIDADE DA PARTIDA", "ENCERRAR PARTIDA"};
        new AlertDialog.Builder(context).setTitle("PAUSA").setItems(options, (dialog, which) -> {
            if (which == 0) { surface.resumeMatch(); whistlePaused = false; }
            else if (which == 1) setRole(1);
            else if (which == 2) { setRole(0); surface.resumeMatch(); whistlePaused = false; }
            else if (which == 3) showSpeedSelector();
            else {
                surface.resetMatch(); setVisibility(GONE);
                if (exitAction != null) exitAction.run();
            }
        }).show();
    }

    private void showSpeedSelector() {
        String[] labels = {"0,5×", "1× NORMAL", "1,5×", "2×"};
        float[] values = {.5f, 1f, 1.5f, 2f};
        new AlertDialog.Builder(context).setTitle("VELOCIDADE")
            .setItems(labels, (d, which) -> surface.setMatchTimeScale(values[which]))
            .setNegativeButton("FECHAR", null).show();
    }

    private GradientDrawable panel() {
        GradientDrawable d = new GradientDrawable();
        d.setColor(Color.argb(218, 5, 17, 31));
        d.setCornerRadius(dp(14));
        d.setStroke(dp(1), Color.argb(150, 26, 190, 205));
        return d;
    }
    private TextView label(String value, int size, boolean bold) {
        TextView t = new TextView(context);
        t.setText(value); t.setTextColor(Color.WHITE); t.setTextSize(size);
        if (bold) t.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        t.setGravity(Gravity.CENTER_VERTICAL);
        return t;
    }
    private Button button(String value) {
        Button b = new Button(context);
        b.setText(value); b.setTextSize(10); b.setTextColor(Color.WHITE); b.setAllCaps(false);
        GradientDrawable d = new GradientDrawable();
        d.setColor(Color.rgb(9, 58, 78)); d.setCornerRadius(dp(11));
        d.setStroke(dp(1), Color.rgb(26, 190, 205));
        b.setBackground(d);
        b.setPadding(dp(3), 0, dp(3), 0);
        return b;
    }
    private int dp(int value) { return Math.round(value * getResources().getDisplayMetrics().density); }
}
