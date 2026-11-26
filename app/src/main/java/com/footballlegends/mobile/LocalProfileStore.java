package com.footballlegends.mobile;

import android.content.Context;
import android.content.SharedPreferences;

/** Offline-first profile storage. No network account is implied by this local profile. */
public final class LocalProfileStore {
    private static final String PREFS = "football_legends_profile";
    private static final String KEY_NAME = "display_name";

    private final SharedPreferences preferences;

    public LocalProfileStore(Context context) {
        preferences = context.getApplicationContext()
                .getSharedPreferences(PREFS, Context.MODE_PRIVATE);
    }

    public String getDisplayName() {
        return preferences.getString(KEY_NAME, "Jogador");
    }

    public boolean saveDisplayName(String name) {
        if (name == null) return false;
        String cleaned = name.trim().replaceAll("\\s+", " ");
        if (cleaned.isEmpty() || cleaned.length() > 20) return false;
        return preferences.edit().putString(KEY_NAME, cleaned).commit();
    }
}