package com.footballlegends.mobile;

import android.app.AlertDialog;
import android.content.Context;
import android.graphics.Color;
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

/** Touch HUD for direct player control and referee decisions. */
public final class MatchUi extends FrameLayout {
    private final GameSurface surface;
    private final Context context;
    private final TextView roleLabel;
    private final LinearLayout actionPanel;
    private int role = 0;
    private int selectedPlayer = 10;
    private boolean whistlePaused = false;

    public MatchUi(Context context, GameSurface surface) {
        super(context);
        this.context = context;
        this.surface = surface;
        setClickable(false);
        setClipChildren(false);
        LinearLayout root = new LinearLayout(context);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setPadding(dp(12), dp(10), dp(12), dp(12));
        addView(root, new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));

        LinearLayout top = new LinearLayout(context);
        top.setGravity(Gravity.CENTER_VERTICAL);
        top.setBackground(panel());
        roleLabel = label("PARTIDA • JOGADOR", 14, Color.WHITE, true);
        top.addView(roleLabel, new LinearLayout.LayoutParams(0, dp(48), 1));
        Button pause = button("PAUSAR");
        pause.setOnClickListener(v -> showPauseMenu());
        top.addView(pause, new LinearLayout.LayoutParams(dp(105), dp(44)));
        root.addView(top);

        View spacer = new View(context);
        root.addView(spacer, new LinearLayout.LayoutParams(1, 0, 1));

        actionPanel = new LinearLayout(context);
        actionPanel.setOrientation(LinearLayout.VERTICAL);
        actionPanel.setPadding(dp(10), dp(8), dp(10), dp(8));
        actionPanel.setBackground(panel());
        root.addView(actionPanel, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        setRole(0);
    }

    public void setRole(int newRole) {
        role = newRole == 1 ? 1 : 0;
        surface.setControlRole(role);
        roleLabel.setText(role == 0 ? "PARTIDA • MODO JOGADOR" : "PARTIDA • MODO ÁRBITRO");
        actionPanel.removeAllViews();
        if (role == 0) buildPlayerControls(); else buildRefereeControls();
    }

    private void buildPlayerControls() {
        actionPanel.addView(label("ESCOLHA O JOGADOR QUE VOCÊ VAI CONTROLAR", 11, Color.WHITE, true));
        Spinner squad = new Spinner(context);
        String[] squadOptions = new String[11];
        for (int i = 0; i < squadOptions.length; i++) squadOptions[i] = "AZUL • CAMISA " + (i + 1);
        squad.setAdapter(new ArrayAdapter<>(context, android.R.layout.simple_spinner_dropdown_item, squadOptions));
        squad.setSelection(10);
        squad.setOnItemSelectedListener(new android.widget.AdapterView.OnItemSelectedListener() {
            @Override public void onItemSelected(android.widget.AdapterView<?> parent, View view, int position, long id) {
                selectedPlayer = position;
                surface.selectPlayer(position);
            }
            @Override public void onNothingSelected(android.widget.AdapterView<?> parent) { }
        });
        actionPanel.addView(squad, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, dp(42)));
        LinearLayout row = new LinearLayout(context);
        row.setGravity(Gravity.CENTER);
        Button left = button("◀");
        Button down = button("▼");
        Button up = button("▲");
        Button right = button("▶");
        addDirection(left, 0, -1);
        addDirection(down, -1, 0);
        addDirection(up, 1, 0);
        addDirection(right, 0, 1);
        for (Button b : new Button[]{left, down, up, right})
            row.addView(b, new LinearLayout.LayoutParams(0, dp(52), 1));
        actionPanel.addView(row);
        LinearLayout actions = new LinearLayout(context);
        actions.setGravity(Gravity.CENTER);
        String[] names = {"PASSE", "CHUTE", "DRIBLE", "CORRER", "DESARME"};
        for (int i = 0; i < names.length; i++) {
            final int action = i;
            Button b = button(names[i]);
            b.setTextSize(9);
            b.setOnClickListener(v -> surface.playerAction(action, -1, 1.0f, 0.0f));
            actions.addView(b, new LinearLayout.LayoutParams(0, dp(46), 1));
        }
        actionPanel.addView(actions);
    }

    private void addDirection(Button button, float x, float y) {
        button.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                surface.movePlayer(x, y);
                return true;
            }
            if (event.getAction() == MotionEvent.ACTION_UP || event.getAction() == MotionEvent.ACTION_CANCEL) {
                surface.movePlayer(0, 0);
                return true;
            }
            return true;
        });
    }

    private void buildRefereeControls() {
        actionPanel.addView(label("SELECIONE O JOGADOR E APLIQUE A DECISÃO", 11, Color.WHITE, true));
        Spinner players = new Spinner(context);
        String[] options = new String[22];
        for (int i = 0; i < options.length; i++) options[i] = (i < 11 ? "AZUL" : "VERMELHO") + " • CAMISA " + ((i % 11) + 1);
        players.setAdapter(new ArrayAdapter<>(context, android.R.layout.simple_spinner_dropdown_item, options));
        players.setSelection(0);
        players.setOnItemSelectedListener(new android.widget.AdapterView.OnItemSelectedListener() {
            @Override public void onItemSelected(android.widget.AdapterView<?> parent, View view, int position, long id) { selectedPlayer = position; }
            @Override public void onNothingSelected(android.widget.AdapterView<?> parent) { }
        });
        actionPanel.addView(players, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, dp(46)));
        LinearLayout cards = new LinearLayout(context);
        Button whistle = button("APITAR / PARAR");
        whistle.setOnClickListener(v -> {
            if (whistlePaused) surface.resumeMatch(); else surface.pauseMatch();
            whistlePaused = !whistlePaused;
            whistle.setText(whistlePaused ? "RETOMAR PARTIDA" : "APITAR / PARAR");
        });
        Button yellow = button("AMARELO");
        yellow.setOnClickListener(v -> surface.refereeIssueCard(selectedPlayer, 1));
        Button red = button("VERMELHO");
        red.setOnClickListener(v -> surface.refereeIssueCard(selectedPlayer, 2));
        cards.addView(whistle, new LinearLayout.LayoutParams(0, dp(48), 1));
        cards.addView(yellow, new LinearLayout.LayoutParams(0, dp(48), 1));
        cards.addView(red, new LinearLayout.LayoutParams(0, dp(48), 1));
        actionPanel.addView(cards);
        actionPanel.addView(label("Cartão amarelo acumula; o segundo resulta em expulsão. Cartão vermelho expulsa imediatamente.", 10, Color.LTGRAY, false));
    }

    private void showPauseMenu() {
        String[] options = {"CONTINUAR", "TROCAR PARA MODO JOGADOR", "TROCAR PARA MODO ÁRBITRO", "ENCERRAR PARTIDA"};
        new AlertDialog.Builder(context).setTitle("PAUSA DA PARTIDA").setItems(options, (dialog, which) -> {
            if (which == 0) {
                if (whistlePaused) { surface.resumeMatch(); whistlePaused = false; }
            } else if (which == 1 || which == 2) {
                surface.pauseMatch();
                setRole(which == 2 ? 1 : 0);
                surface.resumeMatch();
                whistlePaused = false;
            } else {
                surface.pauseMatch();
                setVisibility(GONE);
            }
        }).show();
    }

    private GradientDrawable panel() {
        GradientDrawable d = new GradientDrawable();
        d.setColor(Color.argb(225, 7, 18, 34));
        d.setCornerRadius(dp(15));
        d.setStroke(dp(1), Color.argb(150, 26, 190, 205));
        return d;
    }
    private TextView label(String value, int size, int color, boolean bold) {
        TextView t = new TextView(context);
        t.setText(value); t.setTextColor(color); t.setTextSize(size);
        if (bold) t.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        t.setGravity(Gravity.CENTER_VERTICAL);
        t.setPadding(dp(7), dp(4), dp(7), dp(4));
        return t;
    }
    private Button button(String value) {
        Button b = new Button(context);
        b.setText(value); b.setTextSize(10); b.setTextColor(Color.WHITE); b.setAllCaps(false);
        GradientDrawable d = new GradientDrawable();
        d.setColor(Color.rgb(12, 62, 82)); d.setCornerRadius(dp(10));
        d.setStroke(dp(1), Color.rgb(26, 190, 205));
        b.setBackground(d);
        return b;
    }
    private int dp(int value) { return Math.round(value * getResources().getDisplayMetrics().density); }
}
